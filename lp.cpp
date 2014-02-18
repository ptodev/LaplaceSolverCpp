#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <Eigen/Sparse>
#include <QImage>

typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;

void buildProblem(std::vector<T>& coefficients, Eigen::VectorXd& b,
                  Eigen::MatrixXf circles, Eigen::MatrixXi ic, Eigen::Vector4f bc_grid);
void saveAsBitmap(const Eigen::VectorXd& x, Eigen::MatrixXi ic, const char* filename);

int main()
{
    // Construct the matrix with the initial conditions
    int x_grid_size, y_grid_size, x_min, x_max, y_min, y_max;
    x_grid_size = 600;
    y_grid_size = 300;
    x_min = -30; x_max = 30;
    y_min = -15; y_max = 15;
    Eigen::MatrixXi ic(3,2);
    ic<<x_grid_size, y_grid_size, x_min, x_max, y_min, y_max;

    std::cout<<"Grid properties: "<<std::endl;
    std::cout<<ic<<std::endl;
    std::cout<<"Total number of unknowns: "<<ic(0,0)*ic(0,1)<<std::endl;

    // Add boundary conditions for the sides of the matrix
    Eigen::Vector4f bc_grid;
    bc_grid<<0,0,0,0; // values for up down, left and right respectively

    // Start recording time
    std::clock_t begin = clock();

    // Circular boundary conditions
    //
    // Template:
    // circle1(x-offset, y-offset, radius,potential)
    //
    // Remember to have a "circles" matrix with as many
    // rows as there are circles!
    Eigen::MatrixXf circles(8,4);
    Eigen::Vector4f circle1(-14,0,2,10);
    Eigen::Vector4f circle2(-10,0,2,10);
    Eigen::Vector4f circle3(-5,0,2,10);
    Eigen::Vector4f circle4(0,0,2,10);
    Eigen::Vector4f circle5(14,0,2,10);
    Eigen::Vector4f circle6(10,0,2,10);
    Eigen::Vector4f circle7(5,0,2,10);
    circles.row(0) = circle1;
    circles.row(1) = circle2;
    circles.row(2) = circle3;
    circles.row(3) = circle4;
    circles.row(4) = circle5;
    circles.row(5) = circle6;
    circles.row(6) = circle7;

    // Assembly
    std::vector<T> coefficients; // list of non-zeros coefficients
    Eigen::VectorXd b(ic(0,0)*ic(0,1)); // the right hand side-vector resulting from the constraints
    buildProblem(coefficients, b, circles, ic, bc_grid);

    std::cout<<"The problem has been built."<<std::endl;

    // Construct the matrix from the set of triplets
    SpMat A(ic(0,0)*ic(0,1), ic(0,0)*ic(0,1));
    A.setFromTriplets(coefficients.begin(), coefficients.end());

    // Solving
    Eigen::SimplicialCholesky<SpMat> chol(A); // performs a Cholesky factorization of A
    Eigen::VectorXd x = chol.solve(b); // use the factorization to solve for the given right hand side

    // End recording of time & output
    std::clock_t end = clock();
    double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;
    std::cout<<"For n of "<<ic(0,0)<<"x"<<ic(0,1)<<", the elapsed time is: "<<elapsed_time<<std::endl;

    // Export the result to a file
//    std::ofstream myfile;
//    myfile.open("data.mat");
//    myfile<<x;
//    myfile.close();
    saveAsBitmap(x, ic, "test.jpg");

    return 0;
}

std::pair <bool,int> circularConditionsCheck(int i, int j, Eigen::MatrixXf circles, Eigen::MatrixXi ic){
    double dx = (ic(1,1)-ic(1,0))/((double)ic(0,0));
    double dy = (ic(2,1)-ic(2,0))/((double)ic(0,1));

    std::pair <bool,int> result;
    result = std::make_pair(false,0);

    // Loop through all of the circles to see
    // if the point is in any of them
    for(int current_circle=0;current_circle<circles.rows();current_circle++){
        if( (pow(((ic(1,0)+i*dx)-circles(current_circle,0)),2) +
             pow(((ic(2,0)+j*dy)-circles(current_circle,1)),2) ) <=
             pow(circles(current_circle,2),2))
        {
            result = std::make_pair(true,current_circle);
            return result;
        }
    }

    return result;
}

void insertCoefficient(int id, int i, int j, double w, std::vector<T>& coeffs,
Eigen::VectorXd& b, Eigen::MatrixXf circles, Eigen::MatrixXi ic, Eigen::Vector4f bc_grid){
    int id1 = i+j*ic(0,0);

    // Check if the point is in a circle
    // and get the id of the circle
    std::pair <bool,int> isCircle;
    if(circles.size() != 0){
        isCircle= circularConditionsCheck(i, j, circles, ic);
    } else isCircle = std::make_pair(false, 0);


    // Check if the point is on the left side
    if(i==-1) b(id) -= w * bc_grid(2);
    // Check if the point is on the right side
    else if(i==ic(0,0)) b(id) -= w * bc_grid(3);
    // Check if the point is on the top side
    else if(j==-1) b(id) -= w * bc_grid(0);
    // Check if the point is on the bottom side
    else if(j==ic(0,1)) b(id) -= w * bc_grid(1);
    // Check if the point is in a circle
    else if(isCircle.first) b(id) -= w*circles(isCircle.second, 3);
    // If everything fails, the point must be unknown
    else coeffs.push_back(T(id,id1,w));
}

void buildProblem(std::vector<T>& coefficients, Eigen::VectorXd& b,
                  Eigen::MatrixXf circles, Eigen::MatrixXi ic, Eigen::Vector4f bc_grid){
    b.setZero();

    // Loop through all of the elements on the grid
    for(int j=0; j<ic(0,1); ++j){
        for(int i=0; i<ic(0,0); ++i){
            // The id number in the vector from
            // the flattened matrix
            int id = i+j*ic(0,0);

            // CHECK IF THE SIZE FUNCTION RUNS
            // WHEN THE SIZE OF CIRCLES IS NOT KNOWN
            // AT COMPILATION TIME
            if(circles.size() != 0){
                // Check if the point is in a circular boundary
                std::pair <bool,int> isCircle;
                isCircle= circularConditionsCheck(i, j, circles, ic);
                if(isCircle.first){
                    coefficients.push_back(T(id,id,1.0));
                    b(id) = circles(isCircle.second,3);
                    continue;
                }
            }
            // Insert the coefficient for all other points
            insertCoefficient(id, i-1,j, -1, coefficients, b, circles, ic, bc_grid);
            insertCoefficient(id, i+1,j, -1, coefficients, b, circles, ic, bc_grid);
            insertCoefficient(id, i,j-1, -1, coefficients, b, circles, ic, bc_grid);
            insertCoefficient(id, i,j+1, -1, coefficients, b, circles, ic, bc_grid);
            insertCoefficient(id, i,j, 4, coefficients, b, circles, ic, bc_grid);
        }
    }
}

void saveAsBitmap(const Eigen::VectorXd& x, Eigen::MatrixXi ic, const char* filename){
    Eigen::Array<unsigned char,Eigen::Dynamic,Eigen::Dynamic> bits = (x*255).cast<unsigned char>();
    QImage img(bits.data(), ic(0,0),ic(0,1), QImage::Format_Indexed8);
    img.setColorCount(256);
    for(int i=0;i<256;i++) img.setColor(i,qRgb(i,i,i));
    img.save(filename);
}

