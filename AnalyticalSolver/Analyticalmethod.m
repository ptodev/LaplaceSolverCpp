function potential1 = attempt(V,n,R)

%defining constants
F=zeros(n+1,1);%matrix of potential
F(1,1)=V;%initial value
d=200;%distance from the sphere
L=R/(2*d);%lambda
ddx=5;%distance from x axis
ddy=0;%distance from y axis

%potential matrix
global Phi;
Phi=zeros(450,300);

dx=1;
dy=1;%step size

%Gmn matrix from the solution
G=eye(n+1);
for i=1:n+1,
    for j=1:n+1
        G(i,j)=G(i,j)-L*(-L)^(i+j-2)*nchoosek(i+j-2,i-1);
    end
end
%solves for Un from the solution
U=GEPivShow(G,F);


i=0;%matrix coordinates
for x=-149:dx:300
    %to see how fast it iterates
    i=i+1;
    j=0;%matrix coordinates
    disp(x);
    for y=-149:dy:150,
        SumN=0;
        j=j+1;
        for N=1:n+1,
            %external sum
            
            first=(R/(sqrt((x-ddx)^2+(y-ddy)^2)))^(N);
            second=(sqrt((x-ddx)^2+(y-ddy)^2)/R)^(N-1);
            SumN=SumN+(first*U(N)-second*(U(N)-F(N)))*P_l(N-1,(x-ddx),(y-ddy));
        end
        %dropping the result into the matrix
        if SumN>10,
            SumN=10;
        end
        if x==ddx && y==ddy,
            SumN=10;
        end
        Phi(i,j)=SumN;
        
    end
end
dlmwrite('datafile.txt',Phi,'delimiter',' ');

v=1:300;
v1=1:450;
[px,py]=gradient(-1*Phi(:,:));
figure
contour(v,v1,Phi(:,:),50), hold on, streamslice(v,v1,px,py), hold off
figure
h=surf(v,v1,Phi)
set (h,'edgecolor','none')
end

function expression = P_l(l,x,y)
    %the evaluation of P_n, legendre polynomial
    %with x=cos(theta)
    syms z
    alpha=1/(factorial(l)*power(2,l));
    f=(z^2-1)^l;
    w=alpha*diff(f,l,z);
    %theta=atan2(x,y);
    z=x/(sqrt(x^2+y^2));
    expression=subs(w);
end
    

