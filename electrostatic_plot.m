
dt=0.001; %size of time step
dx=0.005;%size of spatial step
Nx = 1/dx;%number of points in x axis 
Ny = 1/dx;%number of points in y axis
Nt = 1/dt;%number of time steps
v=10; %voltage on charged plate or point etc.
r=0.05*1/dx; %radius of the circle

u=zeros(Nx+1,Ny+1,Nt);%creates empty array to take values of u(j,k,n) where u is the electrostatic function

for n=1:(Nt+1)%used to initialise all variables and set boundary conditions for all time
    
    for ii=1:Nx
        for jj=1:Ny
            if ((ii-Ny/10)^2+(jj-Nx/2)^2)<=r^2
                u(ii,jj,n) = v; 
            end
        end
    end
    %creates circle charge centered 10th the down and half the
    %way to the right
end


for n=1:Nt %works forward in time to generate points
    for k=2:Ny
        for j=2:Nx        
            u(j,k,n+1) = (1/4)*(u(j+1,k,n)+u(j-1,k,n)+u(j,k+1,n)+u(j,k-1,n));%finds the values of u using surrounding points     
        end
    end
    
    
    %ensures charged points retain values if new boundary 
    %conditions are introduced they must also be included 
    %here to ensure values are retained
    for ii=1:Nx
        for jj=1:Ny
            if ((ii-Ny/10)^2+(jj-Nx/2)^2)<=r^2
                u(ii,jj,n+1) = v;
            end
        end
    end
    
end

v=0:dx:1;
[x,y]=meshgrid(v);
%the -1 here is put for the usual convention.
[px,py]=gradient(-1*u(:,:,Nt));
contour(v,v,u(:,:,Nt),30), hold on, streamslice(v,v,px,py), hold off

