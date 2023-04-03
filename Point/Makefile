include ../defs.mk

APPLICATION := points

all: $(APPLICATION)

$(APPLICATION): main.c $(LIBRARY)
	@echo "Building $(APPLICATION) example application"
	@$(CC) $(CFLAGS) main.c -o $(APPLICATION) $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

point.o: point.c point.h
	@$(CC) -c -o point.o point.c $(CFLAGS)

clean:
	@-rm -f points point.o

include ../library.mk
