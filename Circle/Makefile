include ../Makefile

all: main.c circle.o
	cd ../Window; make window.o
	gcc -o circle main.c ../Window/window.o circle.o $(CFLAGS) -I../ -lX11

circle.o: circle.c circle.h
	gcc -c -o circle.o circle.c -I../ $(CFLAGS) -lX11
