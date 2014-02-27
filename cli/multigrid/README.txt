To install, compile solver.cpp:
g++ solver.cpp -o solver
In Linux, run with the command:
$ ./run properties.txt data.txt

If you run a linux VM, you need to install matplotlib:
sudo apt-get install python-matplotlib

If you run NOMACHINE, the plotting file will fail.
In plot.py replace the line:
plt.streamplot(x, y, U, V, color=speed, linewidth=2, cmap=plt.cm.autumn)
with this:
plt.quiver(x, y, U, V)
It doesn't plot the lines of the field, just the arrows.

If you run windows, you can try installing Anaconda Python to run plot.py.
The cpp file should compile with any compiler, but the bash probably won't work, and you need to call the programs manually.
Alternatively, install a Linux VM from Vmware Player or VirtualBox.
