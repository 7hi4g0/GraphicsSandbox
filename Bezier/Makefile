all: main.c
	cd ../Window; make window.o
	cd ../Line; make line.o
	gcc -o bezier main.c ../Window/window.o ../Line/line.o -std=gnu99 -I../ -lX11 -lm
