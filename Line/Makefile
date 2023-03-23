include ../Makefile

all: main.c line.o
	clang -o line main.c ../Window/window.o line.o $(CFLAGS) -I../ -framework Cocoa -lm

line.o: line.c line.h ../Image/image.h ../Point/point.h
	clang -c -o line.o line.c $(CFLAGS) -I../
