include ../Makefile

all: alpha image.o

alpha: alpha.c image.o
	cd ../Window; make window.o
	gcc -o alpha alpha.c image.o ../Window/window.o $(CFLAGS) -I../ -lX11

image.o: image.c image.h
	gcc -c -o image.o image.c $(CFLAGS) -I../
