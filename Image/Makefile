all: alpha image.o

alpha: alpha.c image.o
	cd ../Window; make window.o
	gcc -o alpha alpha.c image.o ../Window/window.o -std=gnu99 -I../ -lX11

image.o: image.c image.h
	gcc -c -o image.o image.c -std=gnu99 -I../
