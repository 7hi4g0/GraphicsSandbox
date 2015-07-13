all: main.c circle.o
	gcc -o circle main.c circle.o -I../ -lX11

circle.o: circle.c circle.h
	gcc -c -o circle.o circle.c -I../ -lX11
