all: memball

memball: main.o
	g++ -o memball -Wall main.o -lproc

main.o: redblack.hpp memball.cpp
	g++ -o main.o -c -Wall memball.cpp
