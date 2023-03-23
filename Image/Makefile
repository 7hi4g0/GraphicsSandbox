include ../Makefile

all: alpha.c image.o
	clang -o alpha alpha.c image.o ../Window/window.o $(CFLAGS) -I../ -framework Cocoa

image.o: image.c image.h
	clang -c -o image.o image.c $(CFLAGS) -I../
