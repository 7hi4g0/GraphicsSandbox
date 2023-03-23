include ../Makefile

all: main.c parametric.o
	clang -o parametric main.c parametric.o ../Window/window.o ../Line/line.o $(CFLAGS) -I../ -framework Cocoa -lm

parametric.o: parametric.c parametric.h ../Image/image.h ../Point/point.h
	clang -c -o parametric.o parametric.c $(CFLAGS) -I../
