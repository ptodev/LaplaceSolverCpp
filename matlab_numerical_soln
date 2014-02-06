tmax=20; %an arbitrarily sized upper limit on time
dt=0.1; %size of time step
d=0.1;%size of spatial step
Nx = 100;%number of points in x axis 
Ny = 100;%number of points in y axis
Nt = tmax/dt;%number of time steps
t=19;%value of time that the graph is plotted at
tu=t/dt +1;%scales the time value to find correct position in array
v=10; %voltage on charged plate or point etc.

x=[1:Nx+1];%creates array containing x values to be used in plotting
y=[1:Ny+1];%same as above but for y

u=zeros(Nx+1,Ny+1,Nt+1);%creates empty array to take values of u(j,k,n) where u is the electrostatic function

for n=1:(Nt+1)%used to initialise all variables and set boundary conditions for all time
    u(5,50,n) = v;  %creates point charge at (5,50)
    for i=1:Ny    
        u(1,i,n) = 0;  %creates grounded plate at lower boundary
    end
end

for n=1:Nt %works forward in time to generate points
    for k=2:Ny
        for j=2:Nx        
            u(j,k,n+1) = (1/4)*(u(j+1,k,n)+u(j-1,k,n)+u(j,k+1,n)+u(j,k-1,n));%finds the values of u using surrounding points     
        end
    end
    
    u(5,50,n+1) = v;  %ensures charged points retain values if new boundary conditions are introduced they must also be included here to ensure values are retained
    for i=1:Ny    
        u(1,i,n+1) = 0;  %retains line of charge  at lower boundary
    end
end

%surf(x*d,y*d,u(:,:,tu));%plots as a 3 dimensional surface
%view(2);%sets the view to 2d 
contour(x*d,y*d,u(:,:,tu),50);%plots as a contour
