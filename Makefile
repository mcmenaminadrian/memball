all: memball

memball: main.o
	g++ -O2 -o memball -Wall main.o -lproc

main.o: redblack.hpp memball.cpp
	g++ -O2 -o main.o -c -Wall memball.cpp
