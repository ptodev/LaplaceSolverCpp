import numpy as np
import sys

D = np.loadtxt("data.txt")
P = np.loadtxt("properties.txt")

grad = np.gradient(D)
U = -1*grad[1]
V = -1*grad[0]
E = np.sqrt(U**2 + V**2)

x = np.linspace(P[0][0], P[0][1], P[0][2])
y = np.linspace(P[1][0], P[1][1], P[1][2])

x_min = P[0][0]
x_max = P[0][1]
x_res = P[0][2]

y_min = P[1][0]
y_max = P[1][1]
y_res = P[1][2]

dx = (x_max-x_min)/x_res
dy = (y_max-y_min)/y_res

print "X sould be between "+str(x_min)+" and "+str(x_max)+"!"
print "Y sould be between "+str(y_min)+" and "+str(y_max)+"!"

while (True):
	print "Type x and y coordinates (ie. 300,100, or q for exit!):"
	coord = sys.stdin.readline().strip()
	if (coord == 'q'):
		break;
	coord = coord.split(",")
	x_query = int(coord[0])
	y_query = int(coord[1])
	if (x_min > x_query or x_max < x_query):
		print "X sould be between "+str(x_min)+" and "+str(x_max)+"!"
		continue;
	if (y_min > y_query or y_max < y_query):
		print "Y sould be between "+str(y_min)+" and "+str(y_max)+"!"
		continue;
	x_index = (x_query-x_min)/dx
	y_index = (y_query-y_min)/dy

	print "V("+str(x_query)+","+str(y_query)+") = "+str(D[y_index,x_index])+" Volts"
	print "E("+str(x_query)+","+str(y_query)+") = "+str(E[y_index,x_index])+" N/C"
