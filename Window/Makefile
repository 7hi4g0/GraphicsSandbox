all: main.c window.o
	gcc -o window main.c window.o -std=gnu11 -I../ -lX11

window.o: window.c window.h
	gcc -c -o window.o window.c -std=gnu11 -I../
