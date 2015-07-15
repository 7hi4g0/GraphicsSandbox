all: main.c window.o
	gcc -o window main.c window.o -I../ -lX11

window.o: window.c window.h
	gcc -c -o window.o window.c -I../
