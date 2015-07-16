all: main.c line.o
	cd ../Window; make window.o
	gcc -o line main.c ../Window/window.o line.o -std=gnu99 -I../ -lX11 -lm

line.o: line.c line.h ../Image/image.h
	gcc -c -o line.o line.c -std=gnu99 -I../ -lX11 -lm
