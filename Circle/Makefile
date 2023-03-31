include ../defs.mk

all: main.c circle.o
	$(CC) -o circle main.c ../Window/window.o circle.o $(CFLAGS) $(GRAPHICS_LINKER)

circle.o: circle.c circle.h
	$(CC) -c -o circle.o circle.c $(CFLAGS)

clean:
	-rm -f circle circle.o
