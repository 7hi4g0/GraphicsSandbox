all: main.c
	cd ../Line; make line.o
	gcc -o bezier main.c ../Line/line.o -std=gnu99 -I.. -lX11
