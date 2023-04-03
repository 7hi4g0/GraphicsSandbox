include ../defs.mk

all: line slope

slope: slope.c $(LIBRARY)
	@echo "Building $@ example application"
	@$(CC) $(CFLAGS) slope.c -o $@ $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

line: main.c $(LIBRARY)
	@echo "Building $@ example application"
	@$(CC) $(CFLAGS) main.c -o $@ $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

line.o: line.c line.h ../Image/image.h ../Point/point.h
	@$(CC) -c -o line.o line.c $(CFLAGS)

clean:
	@-rm -f line line.o

include ../library.mk
