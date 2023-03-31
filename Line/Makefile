include ../defs.mk

all: main.c line.o
	$(CC) -o line main.c ../Window/window.o line.o $(CFLAGS) $(GRAPHICS_LINKER) -lm

line.o: line.c line.h ../Image/image.h ../Point/point.h
	$(CC) -c -o line.o line.c $(CFLAGS)

clean:
	-rm -f line line.o
