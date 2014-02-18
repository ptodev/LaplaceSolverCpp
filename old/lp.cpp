#include <Eigen/Sparse>
#include <vector>
#include <QImage>
#include <iostream>
#include <fstream>
#include <ctime>

typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;

void buildProblem(std::vector<T>& coefficients, Eigen::VectorXd& b, int n);
void saveAsBitmap(const Eigen::VectorXd& x, int n, const char* filename);

int main(int argc, char** argv)
{
    // start recording time
    std::clock_t begin = clock();
    int n = 300; // size of the image
    int m = n*n; // number of unknows (=number of pixels)

    // Assembly:
    std::vector<T> coefficients; // list of non-zeros coefficients
    Eigen::VectorXd b(m); // the right hand side-vector resulting from the constraints
    buildProblem(coefficients, b, n);

    SpMat A(m, m);
    A.setFromTriplets(coefficients.begin(), coefficients.end());

    // Solving:
    Eigen::SimplicialCholesky<SpMat> chol(A); // performs a Cholesky factorization of A
    Eigen::VectorXd x = chol.solve(b); // use the factorization to solve for the given right hand side

    // End recording of time & output
    std::clock_t end = clock();
    double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;
    std::cout<<"For n of "<<n<<", the elapsed time is: "<<elapsed_time<<std::endl;

    // Export the result to a file:
    std::ofstream myfile;
    myfile.open("data.mat");
    myfile<<x;
    myfile.close();
    saveAsBitmap(x, n, "test.jpg");

    return 0;
}

void insertCoefficient(int id, int i, int j, double w, std::vector<T>& coeffs,
Eigen::VectorXd& b, const Eigen::VectorXd& boundary)
{
    double dx = 30.0/300.0;
    int n = boundary.size();
    int id1 = i+j*n;
    if(i==-1 || i==n) b(id) -= w * 0.0; // constrained coefficient
    else if(j==-1 || j==n) b(id) -= w * 0.0; // constrained coefficient
    else if( (pow(((-15.0+i*dx)-0.0),2) + pow(((-15.0+j*dx)-0.0),2) ) <= pow(3.0,2)) b(id) -= w*10.0;
    else coeffs.push_back(T(id,id1,w)); // unknown coefficient
}

void buildProblem(std::vector<T>& coefficients, Eigen::VectorXd& b, int n)
{
    b.setZero();
    Eigen::ArrayXd boundary = Eigen::ArrayXd::Ones(n)*0.0;
    for(int j=0; j<n; ++j)
    {
        for(int i=0; i<n; ++i)
        {
            int id = i+j*n;
            double dx = 30.0/300.0;
            if( (pow(((-15.0+i*dx)-0.0),2) + pow(((-15.0+j*dx)-0.0),2) ) <= pow(3.0,2))
            {
                coefficients.push_back(T(id,id,1.0));
                b(id) = 10.0;
                continue;
            }

            insertCoefficient(id, i-1,j, -1, coefficients, b, boundary);
            insertCoefficient(id, i+1,j, -1, coefficients, b, boundary);
            insertCoefficient(id, i,j-1, -1, coefficients, b, boundary);
            insertCoefficient(id, i,j+1, -1, coefficients, b, boundary);
            insertCoefficient(id, i,j, 4, coefficients, b, boundary);
        }
    }
}

void saveAsBitmap(const Eigen::VectorXd& x, int n, const char* filename)
{
    Eigen::Array<unsigned char,Eigen::Dynamic,Eigen::Dynamic> bits = (x*255).cast<unsigned char>();
    QImage img(bits.data(), n,n,QImage::Format_Indexed8);
    img.setColorCount(256);
    for(int i=0;i<256;i++) img.setColor(i,qRgb(i,i,i));
    img.save(filename);
}

