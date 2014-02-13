g++ sim.cpp -o sim
./sim

gnuplot -persist <<- EOF
	set pm3d map
	splot './U.mat' matrix
EOF

