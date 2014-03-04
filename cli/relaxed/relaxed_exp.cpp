#include <iostream>
#include <iomanip>//used for displaying matrix not necessary
#include <cmath>//used for abs function
#include <fstream>
#include <cstdlib>

using namespace std;

int main(){

  int Nx=150;//number of points in the x direction
  int Ny=20;//number of points in the y direction
  int Nt=100000;//number of iterations
  double TOL = 0.000000001;//set tolerance
  double v=10;//voltage of a point, can add more values (e.g. v1, v2, ...) as required
  double sigma=1.2;//relaxation coefficient
  char relaxated_out_file[Nt];
  int end_value = 0;

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

  for (int jj=0; jj<Nx;jj++){
    u[0][jj]=-v;
  }
  
  for (int jj=0; jj<Nx; jj++){
    u[(Ny-1)][jj]=-v;
  }

  u[10][62]=0;
  u[10][75]=0;
  u[10][88]=0;
 
  //u[10][10]=v;//sets a central point to v, currently just being used as an example

  for (int t=1; t<Nt; t++){//uses the laplacian to find values
    if (end_value==1){
      cout << "Number of iterations: " << t << endl;
      cout << "Tolerance: " << TOL << endl;
      break;
    }
    for (int i=0; i<Ny; i++){
      for (int j=0; j<Nx; j++){
	double copy=u[i][j];//needed to find absolute difference for tolerance
	if (j==0){
	  if (i==0){
	    u[i][j]=((1-sigma)*u[i][j])+(sigma*(0.5)*(u[i+1][j]+u[i][j+1]));
	  }
	  else if (i==(Ny-1)){
	    u[i][j]=((1-sigma)*u[i][j])+(sigma*(0.5)*(u[i-1][j]+u[i][j-1]));
	  }
	  else{
	    u[i][j]=((1-sigma)*u[i][j])+((sigma/3)*(u[i+1][j]+u[i][j+1]+u[i-1][j]));
	  }	 
	}
	else if (j==(Nx-1)){
	  if (i==0){
	    u[i][j]=((1-sigma)*u[i][j])+(sigma*(0.5)*(u[i+1][j]+u[i][j-1]));
	    //cout << "1" << endl;
	  }
	  else if(i==(Ny-1)){
	    u[i][j]=((1-sigma)*u[i][j])+(sigma*(0.5)*(u[i-1][j]+u[i][j-1]));
	    //cout << "2" << endl;
	  }	    
	  else{
	    u[i][j]=((1-sigma)*u[i][j])+((sigma/3)*(u[i+1][j]+u[i-1][j]+u[i][j-1]));
	  }
	}

	else if (i==0 && j>0 && j<(Nx-1)){
	  u[i][j]=((1-sigma)*u[i][j])+((sigma/3)*(u[i+1][j]+u[i][j-1]+u[i][j+1]));
	}	  
	else if (i==(Ny-1) && j>0 && j<(Nx-1)){
	  u[i][j]=((1-sigma)*u[i][j])+((sigma/3)*(u[i-1][j]+u[i][j-1]+u[i][j+1]));
	}	  
	else{
	  u[i][j]=((1-sigma)*u[i][j])+(sigma*(0.25)*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]));
	}

	for (int jj=0; jj<Nx;jj++){
	  u[0][jj]=-v;
	}
  
	for (int jj=0; jj<Nx; jj++){
	  u[(Ny-1)][jj]=-v;
	}

	u[10][62]=0;
	u[10][75]=0;
	u[10][88]=0;
 
	//u[10][10]=v;//here need to reset boundary conditions

	//cout << "i= " << i << " j= " << j << endl;
	
	if (abs(copy-u[i][j])<TOL && abs(copy-u[i][j])>0){
	  end_value=1;  
	}
       
      }
    }
  }	  
  
  if (end_value==0){
    cout << "Maximum number of iterations reached!" << endl;
    cout << "Number of iterations: " << Nt << endl;
    cout << "Tolerance: " << TOL << endl;
  }    
 
  ofstream relaxed_out("data.txt");//outputs generated matrix to a text file data.txt
  for (int i=0; i<Ny; i++){
    for (int j=0; j<Nx; j++){ 
      relaxed_out << u[i][j] << " ";
    }
    relaxed_out << "\n";
  }
  relaxed_out.close();

  ofstream properties_out("properties.txt");//outputs dimensions of matrix etc to properties.txt to be used by plot.py
  properties_out << "0" << " " << Nx-1 << " " << Nx << "\n" << "0" << " " << Ny << " " << Ny;
  properties_out.close(); 
  
  
}
