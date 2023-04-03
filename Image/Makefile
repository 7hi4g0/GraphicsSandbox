include ../defs.mk

APPLICATION := alpha

all: $(APPLICATION)

$(APPLICATION): alpha.c $(LIBRARY)
	@echo "Building $(APPLICATION) example application"
	@$(CC) $(CFLAGS) alpha.c -o $(APPLICATION) $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics

image.o: image.c image.h
	@$(CC) -c -o image.o image.c $(CFLAGS)

clean:
	@-rm -f alpha image.o

include ../library.mk
