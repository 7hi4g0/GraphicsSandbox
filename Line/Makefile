include ../Makefile

all: main.c line.o
	cd ../Window; make window.o
	gcc -o line main.c ../Window/window.o line.o $(CFLAGS) -I../ -lX11 -lm

line.o: line.c line.h ../Image/image.h
	gcc -c -o line.o line.c $(CFLAGS) -I../ -lX11 -lm
