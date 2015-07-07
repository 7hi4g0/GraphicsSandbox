all: main.c line.o
	gcc -o line main.c line.o -std=gnu99 -I../ -lX11 -lm

line.o: line.c line.h ../Image/image.h
	gcc -c -o line.o line.c -std=gnu99 -I../ -lX11 -lm
