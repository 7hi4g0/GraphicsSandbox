include ../defs.mk

all: alpha.c image.o
	$(CC) -o alpha alpha.c image.o ../Window/window.o $(CFLAGS) $(GRAPHICS_LINKER)

image.o: image.c image.h
	$(CC) -c -o image.o image.c $(CFLAGS)

clean:
	-rm -f alpha image.o
