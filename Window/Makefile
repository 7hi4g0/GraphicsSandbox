all: main.c window.o
	gcc -o window main.c window.o -std=gnu99 -I../ -lX11

window.o: window.c window.h
	gcc -c -o window.o window.c -std=gnu99 -I../
