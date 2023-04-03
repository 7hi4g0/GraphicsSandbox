include ../defs.mk

APPLICATION := bezier

all: $(APPLICATION)

$(APPLICATION): main.c $(LIBRARY)
	@echo "Building $(APPLICATION) example application"
	@$(CC) $(CFLAGS) main.c -o $(APPLICATION) $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

clean:
	@-rm -f bezier

include ../library.mk
