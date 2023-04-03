include ../defs.mk

APPLICATION := parametric

all: $(APPLICATION)

$(APPLICATION): main.c $(LIBRARY)
	@echo "Building $(APPLICATION) example application"
	@$(CC) $(CFLAGS) main.c -o $(APPLICATION) $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

parametric.o: parametric.c parametric.h ../Image/image.h ../Point/point.h
	@$(CC) -c -o parametric.o parametric.c $(CFLAGS)

clean:
	@-rm -f parametric parametric.o

include ../library.mk
