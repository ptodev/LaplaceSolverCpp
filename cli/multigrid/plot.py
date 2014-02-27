import numpy as np
import matplotlib.pyplot as plt

D = np.loadtxt("data.txt")
P = np.loadtxt("properties.txt")

grad = np.gradient(D)
U = -1*grad[1]
V = -1*grad[0]

x = np.linspace(P[0][0], P[0][1], P[0][2])
y = np.linspace(P[1][0], P[1][1], P[1][2])

speed = np.sqrt(U*U + V*V)

plt.streamplot(x, y, U, V, color=speed, linewidth=2, cmap=plt.cm.autumn)

CS = plt.contour(x, y, speed)
plt.clabel(CS, inline=1, fontsize=10)

plt.axes().set_aspect('equal')
plt.show()
plt.ylim([P[0][0],P[0][1]])
plt.xlim([P[1][0],P[1][1]])
