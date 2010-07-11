memball: redblack.o memball.o
	g++ -c -Wall redblack.cpp
	g++ -c -Wall memball.cpp
	g++ -o memball redblack.o memball.o -lproc
