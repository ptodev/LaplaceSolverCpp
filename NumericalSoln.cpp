#include <iostream>
#include <iomanip>//used for displaying matrix not necessary
#include <cmath>//used for abs function

int main(){
  using namespace std;

  int Nx=8;//number of points in the x direction
  int Ny=8;//number of points in the y direction
  int Nt=2000;//number of iterations
  double TOL = 0.00001;//set tolerance
  double v=30;//voltage of a point, can add more values (e.g. v1, v2, ...) as required
  double sigma=0.4;//relaxation coefficient
 
  double **u;//creates dynamically assignable matrix containing potential values 
  u = new double*[Ny];
  for(int i=0; i<Ny; i++){
    u[i]=new double[Nx];
  }
  
  for (int i=0; i<Ny; i++){//sets all values in matrix to 0
    for (int j=0; j<Nx; j++){
      u[i][j]=0;
    }
  }

  u[4][4]=v;//sets a central point to v, currently just being used as an example

  for (int t=0; t<Nt; t++){//uses the laplacian to find values
    for (int i=1; i<(Ny-1); i++){
      for (int j=1; j<(Nx-1); j++){
	double copy=u[i][j];//needed to find absolute difference for tolerance
	u[i][j]=((1-sigma)*u[i][j])+(sigma*0.25*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]));
	u[4][4]=v;//here need to reset boundary conditions
	if (abs(copy-u[i][j])<TOL && abs(copy-u[i][j])>0){
	  cout << "Number of iterations: " << t << endl;
	  cout << "Tolerance: " << TOL << endl;
	  goto stop;
	}
      }
    }
  }

  stop:  
  for (int iii=1; iii<Ny-1; iii++){//prints matrix
    for (int jjj=1; jjj<Nx-1; jjj++){
      cout << fixed;
      cout << setprecision(4) << " " << u[iii][jjj] << " ";
    }
    cout << endl;
  }
}
