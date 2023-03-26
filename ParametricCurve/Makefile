include ../defs.mk

all: main.c parametric.o
	$(CC) -o parametric main.c parametric.o ../Window/window.o ../Line/line.o $(CFLAGS) $(GRAPHICS_LINKER) -lm

parametric.o: parametric.c parametric.h ../Image/image.h ../Point/point.h
	$(CC) -c -o parametric.o parametric.c $(CFLAGS)

clean:
	-rm -f parametric parametric.o
