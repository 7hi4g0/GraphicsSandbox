all: main.c point.o
	cd ../Window; make window.o
	cd ../Line; make line.o
	gcc -o points main.c point.o ../Window/window.o ../Line/line.o -std=gnu11 -I../ -lX11 -lm

point.o: point.c point.h
	gcc -c -o point.o point.c -std=gnu11 -I../
