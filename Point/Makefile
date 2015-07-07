all: main.c point.o
	cd ../Line; make line.o
	gcc -o points main.c point.o ../Line/line.o -std=gnu99 -I../ -lX11

point.o: point.c point.h
	gcc -c -o point.o point.c -I../
