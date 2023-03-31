include ../defs.mk

all: main.c
	$(CC) -o bezier main.c ../Window/window.o ../Line/line.o $(CFLAGS) $(GRAPHICS_LINKER) -lm

clean:
	-rm -f bezier
