#include <iostream>
#include <math.h>
#include <armadillo>

using namespace std;
using namespace arma;

#define XSIZE 300

pair<double, double> mapping(int i, int j, double dx, double x_min){
double x,y;
pair<double, double> result;

result.first = x_min + j*dx;
result.second = x_min + i*dx;

return result;
}

mat putCircleBoundary(mat matrix, double a, double b, double r, double boundary, double dx, double xmin){
pair<double, double> coordinates;
for(int row=0; row<matrix.n_rows; row++){
for(int col=0; col<matrix.n_cols; col++){
coordinates = mapping(row, col, dx, xmin);
if (( pow((coordinates.first-a),2) + pow((coordinates.second-b),2) ) < pow(r,2) ){
matrix(row,col) = boundary;
}
}
}
return matrix;
}

int main(){
double x_min, x_max, dx;
mat::fixed<300, 300> U;
const int x_elements = ceil((x_max - x_min)/dx);

x_min = -15.0;
x_max = 15.0;
dx = 0.1;

U.zeros();
U = putCircleBoundary(U, 0.0, 0.0, 4.0, 10.0, dx, x_min);

// Creating such a large matrix causes a segmentation fault
// mat::fixed<90000, 90000> SYS;
// for(int row=1; row<(U.n_rows-1); row++){
//  for(int col=1; col<(U.n_cols-1); col++){
//    
//  }
// }

U.save("U.mat", raw_ascii);

return 0;
}
