function potential1 = attempt(V,n,R)

%defining constants
%n=20;%sum to
%V=10;%potential
F=zeros(n,1);%matrix of potential
F(1,1)=V;%initial value
%R=1;%radius
d=2;%distance from the sphere
L=R/(2*d);%lambda

%Gmn matrix from the solution
g=ones(n,1);
G=diag(g,0);
for i=1:n,
    for j=1:n
        G(i,j)=G(i,j)-(-L)^(i+j-2)*nchoosek(i+j-2,i-1);
    end
end
%solves for Un from the solution
U=GEPivShow(G,F);
%potential matrix
Phi=zeros(30,30);

%coordinates

for x=1:30
    %to see how fast it iterates
    disp(x);
    for y=1:30,
        SumN=0;
        for N=1:n,
            SumM=0;
            %internal sum
            for M=1:n,
                SumM=SumM+U(M)*G(N,M);
            end
            %external sum
            first=(R/(sqrt(x^2+y^2)))^(N+1);
            second=(sqrt(x^2+y^2)/R)^N;
            SumN=SumN+(first*U(N)-second*L*SumM)*P_l(N,x,y);
        end
        %dropping the result into the matrix
        Phi(x,y)=SumN;
    end
end
surf(Phi);


end

function expression = P_l(l,x,y)
    %the evaluation of P_n, legendre polynomial
    %with x=cos(theta)
    syms z
    alpha=1/(factorial(l)*power(2,l));
    f=(z^2-1)^l;
    w=alpha*diff(f,l,z);
    theta=atan2(x,y);
    z=cos(theta);
    expression=subs(w);
end

function theta = atan2(x,y)
    %the evaluation of the common variation on the arctangent
    %function
    if x>0
        theta=atan(y/x);
    else if x<0 && y>=0
            theta=atan(y/x)+pi;
        else if x<0 && y<0,
                theta=atan(y/x)-pi;
            else if x==0 && y>0
                    theta=pi/2;
                else if x==0 && y<0,
                        theta=-pi/2;
                    end
                end
            end
        end
    end
end
    

