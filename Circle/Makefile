include ../defs.mk

APPLICATION := circle

all: $(APPLICATION)

$(APPLICATION): main.c $(LIBRARY)
	@echo "Building $(APPLICATION) example application"
	@$(CC) $(CFLAGS) main.c -o $(APPLICATION) $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics

circle.o: circle.c circle.h
	@$(CC) -c -o circle.o circle.c $(CFLAGS)

clean:
	@-rm -f circle circle.o

include ../library.mk
