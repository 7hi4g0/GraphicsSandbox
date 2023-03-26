include ../defs.mk

all: main.c point.o
	$(CC) -o points main.c point.o ../Window/window.o ../Line/line.o $(CFLAGS) $(GRAPHICS_LINKER) -lm

point.o: point.c point.h
	$(CC) -c -o point.o point.c $(CFLAGS)

clean:
	-rm -f points point.o
