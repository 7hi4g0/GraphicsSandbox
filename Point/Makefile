include ../Makefile

all: main.c point.o
	clang -o points main.c point.o ../Window/window.o ../Line/line.o $(CFLAGS) -I../ -framework Cocoa -lm

point.o: point.c point.h
	clang -c -o point.o point.c $(CFLAGS) -I../
