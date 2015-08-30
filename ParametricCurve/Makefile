include ../Makefile

all: main.c parametric.o
	cd ../Window; make window.o
	cd ../Line; make line.o
	gcc -g -o parametric main.c parametric.o ../Window/window.o ../Line/line.o $(CFLAGS) -I../ -lX11 -lm

parametric.o: parametric.c parametric.h ../Image/image.h ../Point/point.h
	gcc -g -c -o parametric.o parametric.c $(CFLAGS) -I../ -lX11 -lm
