include ../Makefile

all: main.c window.o
	clang -o window main.c window.o -I../ -framework Cocoa

window.o: window.m window.h
	clang -c -o window.o window.m -I../
