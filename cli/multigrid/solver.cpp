#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <cassert>
#include <stdio.h>
#include "Eigen/Sparse"

typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;

Eigen::MatrixXf fillMatricesFromFile(Eigen::MatrixXi& ic, Eigen::Vector4f& bc_grid, char* properties_file_name);
void buildProblem(std::vector<T>& coefficients, Eigen::VectorXd& b,
		Eigen::MatrixXf bc_internal, Eigen::MatrixXi ic, Eigen::Vector4f bc_grid);

int main(int argc, char* argv[])
{
	/******************************************************
	 * VERIFY THAT THERE ARE TWO ARGUMENTS TO THE PROGRAM *
	 ******************************************************/
	if(argc!=3){
		std::cout<<"Please call the function this way:"<<std::endl;
		std::cout<<"./solve input_file.txt output_file.txt"<<std::endl;
		return 1;
	}

	std::cout<<"####### RUNNING MULTIGRID #########"<<std::endl;

	/******************************************************
	 ***** READ THE DATA FROM THE PROPERTIES.TXT FILE *****
	 ******************************************************/
	Eigen::MatrixXi ic(3,2);
	Eigen::Vector4f bc_grid;
	Eigen::MatrixXf bc_internal = fillMatricesFromFile(ic, bc_grid, argv[1]);

	/******************************************************
	 **************** START RECORDING TIME ****************
	 ******************************************************/
	std::clock_t begin = clock();

	/******************************************************
	 ***************** BUILD THE PROBLEM ******************
	 ******************************************************/
	std::vector<T> coefficients; // list of non-zeros coefficients
	Eigen::VectorXd b(ic(0,0)*ic(0,1)); // the right hand side-vector resulting from the constraints
	buildProblem(coefficients, b, bc_internal, ic, bc_grid);
	std::cout<<"THE PROBLEM HAS BEEN BUILT!"<<std::endl;

	/******************************************************
	 ************ CONSTRUCT THE SPARSE MATRIX *************
	 ************** AND SOLVE THE PROBLEM *****************
	 ******************************************************/
	// Construct the matrix from the set of triplets
	SpMat A(ic(0,0)*ic(0,1), ic(0,0)*ic(0,1));
	A.setFromTriplets(coefficients.begin(), coefficients.end());

	// Solving
	Eigen::SimplicialCholesky<SpMat> chol(A); // performs a Cholesky factorization of A
	Eigen::VectorXd x = chol.solve(b); // use the factorization to solve for the given right hand side
	std::cout<<"THE MATRIX HAS BEEN SOLVED!"<<std::endl;

	/******************************************************
	 ***************** END RECORDING TIME *****************
	 ******************************************************/
	std::clock_t end = clock();
	double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;
	std::cout<<"For n of "<<ic(0,0)<<"x"<<ic(0,1)<<", the elapsed time is: "<<elapsed_time<<std::endl;

	/******************************************************
	 *********** CONSTRUCT THE FINAL MATRIX ***************
	 ************ FROM THE FLATTENED VECTOR ***************
	 ******************************************************/
	int x_grid_size = ic(0,0);
	int y_grid_size = ic(0,1);
	Eigen::MatrixXd result(y_grid_size,x_grid_size);
	for(int j=0;j<y_grid_size;j++){
		for(int i=0;i<x_grid_size;i++){
			result(j,i) = x(i + j*x_grid_size);
		}
	}

	/******************************************************
	 *************** Export matrix to file ****************
	 ******************************************************/
	std::ofstream data_file;
	// IFSTREAM accepts only a char, so firstly convert the string
	// data_file.open( output_file_name.c_str() );
	data_file.open( argv[2] );
	data_file<<result;
	data_file.close();

	std::cout<<"######## MULTIGRID ENDED ##########"<<std::endl;

	return 0;
}

/*********************************************************************************************************
 *********************************** START DEFINITIONS OF FUNCTIONS **************************************
 *********************************************************************************************************/


Eigen::MatrixXf fillMatricesFromFile(Eigen::MatrixXi& ic, Eigen::Vector4f& bc_grid, char* properties_file_name){
	/******************************************************
	 ****************** A FUNCTION TO *********************
	 ******* FILL THE DATA FROM THE PROPERTIES FILE *******
	 ******************************************************/

	// Open the file
	// IFSTREAM accepts only a char, so firstly convert the string
	std::ifstream properties_file( properties_file_name );
	assert(properties_file.is_open());

	// Imort the properties of the grid
	// FIRST TWO LINES OF PROPERTIES FILE
	int x_grid_size, y_grid_size, x_min, x_max, y_min, y_max;
	properties_file >> x_min; properties_file >> x_max; properties_file >> x_grid_size;
	properties_file >> y_min; properties_file >> y_max; properties_file >> y_grid_size;
	ic<<x_grid_size, y_grid_size, x_min, x_max, y_min, y_max;

	// Import the boundary conditions for the sides of the matrix
	// THIRD LINE OF PROPERTIES FILE
	float bc_up, bc_down, bc_left, bc_right;
	properties_file >> bc_up;
	properties_file >> bc_down; 
	properties_file >> bc_left; 
	properties_file >> bc_right;
	bc_grid<< bc_up, bc_down, bc_left, bc_right; // values for up down, left and right respectively

	// Import the boundary conditions for the bc_internal
	// THE REST OF THE LINES OF THE PROPERTIES FILE
	std::vector<std::vector <float> > bc_internal_temp;
	std::vector <float> circle_temp;
	float bc_type, x_offset, y_offset, radius, voltage, last;
	while(!properties_file.eof()){
		circle_temp.erase(circle_temp.begin(), circle_temp.end());

		properties_file >> bc_type;  circle_temp.push_back(bc_type);
		properties_file >> x_offset; circle_temp.push_back(x_offset);
		properties_file >> y_offset; circle_temp.push_back(y_offset);
		properties_file >> radius;   circle_temp.push_back(radius);
		properties_file >> voltage;  circle_temp.push_back(voltage);
		properties_file >> last;     circle_temp.push_back(last);

		bc_internal_temp.push_back(circle_temp);
	}
	Eigen::MatrixXf bc_internal ((bc_internal_temp.size()-1),6);
	// Convert the STL vectors into and EIGIEN matrix
	for(int i=0;i<(bc_internal_temp.size()-1);i++){
		for(int j=0;j<6;j++){
			bc_internal(i,j) = bc_internal_temp[i][j];
		}
	}

	// Close the properties.txt file
	properties_file.close();

	return bc_internal;
}

std::pair <int,int> circularConditionsCheck(int i, int j, Eigen::MatrixXf bc_internal, Eigen::MatrixXi ic){
	/******************************************************
	 ****************** A FUNCTION TO *********************
	 *********** CHECK IF A POINT IS IN A CIRCLE **********
	 ******************************************************/
	double dx = (ic(1,1)-ic(1,0))/((double)ic(0,0));
	double dy = (ic(2,1)-ic(2,0))/((double)ic(0,1));

	std::pair <int,int> result;
	result = std::make_pair(0,0);
	double x = (ic(1,0)+i*dx);
	double y = (ic(2,0)+j*dy);
	// Loop through all of the bc_internal to see
	// if the point is in any of them
	for(int current_circle=0;current_circle<bc_internal.rows();current_circle++){
		// If statement for CIRCULAR conditions
		if (bc_internal(current_circle,0) == 0){
			if( (pow((x-bc_internal(current_circle,1)),2) +
						pow((y-bc_internal(current_circle,2)),2) ) <=
					pow(bc_internal(current_circle,3),2))
			{
				result = std::make_pair(1,current_circle);
				return result;
			}
		}
		// If statements for RECTANGULAR conditions
		if (bc_internal(current_circle,0) == 1){
			if( (bc_internal(current_circle,1) <= x && x <= bc_internal(current_circle,3)) &&
					bc_internal(current_circle,2) <= y && y <= bc_internal(current_circle,4))
			{
				result = std::make_pair(2,current_circle);
				return result;
			}
		}
	}

	return result;
}

void insertCoefficient(int id, int i, int j, double w, std::vector<T>& coeffs,
		Eigen::VectorXd& b, Eigen::MatrixXf bc_internal, Eigen::MatrixXi ic, Eigen::Vector4f bc_grid){
	/******************************************************
	 ************** A FUNCTION TO FILL THE ****************
	 ************ COEFFICIENTS AND B VECTORS **************
	 ******************************************************/
	int id1 = i+j*ic(0,0);

	// Check if the point is in a circle
	// and get the id of the circle
	std::pair <int,int> isBcInternal;
	if(bc_internal.size() != 0){
		isBcInternal= circularConditionsCheck(i, j, bc_internal, ic);
	} else isBcInternal = std::make_pair(0, 0);


	// Check if the point is on the left side
	if(i==-1) b(id) -= w * bc_grid(2);
	// Check if the point is on the right side
	else if(i==ic(0,0)) b(id) -= w * bc_grid(3);
	// Check if the point is on the top side
	else if(j==-1) b(id) -= w * bc_grid(0);
	// Check if the point is on the bottom side
	else if(j==ic(0,1)) b(id) -= w * bc_grid(1);
	// Check if the point is in a circle
	else if(isBcInternal.first == 1) b(id) -= w*bc_internal(isBcInternal.second, 4);
	// Check if the point is in a rectangle
	else if(isBcInternal.first == 2) b(id) -= w*bc_internal(isBcInternal.second, 5);
	// If everything fails, the point must be unknown
	else coeffs.push_back(T(id,id1,w));
}

void buildProblem(std::vector<T>& coefficients, Eigen::VectorXd& b,
		Eigen::MatrixXf bc_internal, Eigen::MatrixXi ic, Eigen::Vector4f bc_grid){
	/******************************************************
	 **************** A FUNCTION TO LOOP AND **************
	 ****** CALL THE INCERT COEFFICIENTS FOR EVERY POINT **
	 ******************************************************/
	b.setZero();

	// Loop through all of the elements on the grid
	for(int j=0; j<ic(0,1); ++j){
		for(int i=0; i<ic(0,0); ++i){
			// The id number in the vector from
			// the flattened matrix
			int id = i+j*ic(0,0);

			// Check if there are any circular boundary conditions
			if(bc_internal.size() != 0){
				// Check if the point is in a circular boundary
				std::pair <int,int> isBcInternal;
				isBcInternal= circularConditionsCheck(i, j, bc_internal, ic);
				if(isBcInternal.first != 0){
					coefficients.push_back(T(id,id,1.0));
					// Condition for bc_internal
					if (isBcInternal.first == 1){
						b(id) = bc_internal(isBcInternal.second,4);
					}
					// Condition for rectangles
					if (isBcInternal.first == 2){
						b(id) = bc_internal(isBcInternal.second,5);
					}
					continue;
				}
			}
			// Insert the coefficient for all other points
			insertCoefficient(id, i-1,j, -1, coefficients, b, bc_internal, ic, bc_grid);
			insertCoefficient(id, i+1,j, -1, coefficients, b, bc_internal, ic, bc_grid);
			insertCoefficient(id, i,j-1, -1, coefficients, b, bc_internal, ic, bc_grid);
			insertCoefficient(id, i,j+1, -1, coefficients, b, bc_internal, ic, bc_grid);
			insertCoefficient(id, i,j, 4, coefficients, b, bc_internal, ic, bc_grid);
		}
	}
}
