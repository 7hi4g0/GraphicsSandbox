include ../Makefile

all: main.c
	clang -o bezier main.c ../Window/window.o ../Line/line.o $(CFLAGS) -I../ -framework Cocoa -lm
