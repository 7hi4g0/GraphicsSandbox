include ../Makefile

all: main.c circle.o
	clang -o circle main.c ../Window/window.o circle.o $(CFLAGS) -I../ -framework Cocoa

circle.o: circle.c circle.h
	clang -c -o circle.o circle.c -I../ $(CFLAGS) -framework Cocoa
