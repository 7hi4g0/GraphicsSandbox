all: main.c image.o
	gcc -o alpha main.c image.o -I../ -lX11

image.o: image.c image.h
	gcc -c -o image.o image.c -I../
