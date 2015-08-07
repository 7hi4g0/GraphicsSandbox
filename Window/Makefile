include ../Makefile

all: main.c window.o
	gcc -o window main.c window.o $(CFLAGS) -I../ -lX11

window.o: window.c window.h
	gcc -c -o window.o window.c $(CFLAGS) -I../
