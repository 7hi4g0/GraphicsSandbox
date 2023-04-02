include ../defs.mk

all: line slope

slope: slope.c line.o
	$(CC) -o slope slope.c ../Window/window.o ../Point/point.o line.o $(CFLAGS) $(GRAPHICS_LINKER) -lm

line: main.c line.o
	$(CC) -o line main.c ../Window/window.o ../Point/point.o line.o $(CFLAGS) $(GRAPHICS_LINKER) -lm

line.o: line.c line.h ../Image/image.h ../Point/point.h
	$(CC) -c -o line.o line.c $(CFLAGS)

clean:
	-rm -f line line.o
