all: main.c
	gcc -o line main.c -std=gnu99 -lX11

line.o: line.c line.h ../Image/image.h
	gcc -c -o line.o line.c -std=gnu99 -I../ -lX11 -lm
