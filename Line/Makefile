include ../defs.mk

EXAMPLE_APPLICATION := line
APPLICATIONS += slope antialias

ALL_APPLICATIONS := $(EXAMPLE_APPLICATION) $(APPLICATIONS)

all: $(ALL_APPLICATIONS)

$(APPLICATIONS): % : %.c $(LIBRARY)
	@echo "Building $@ example application"
	@$(CC) $(CFLAGS) $< -o $@ $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

$(EXAMPLE_APPLICATION): main.c $(LIBRARY)
	@echo "Building $@ example application"
	@$(CC) $(CFLAGS) main.c -o $@ $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics -lm

line.o: line.c line.h ../Image/image.h ../Point/point.h
	@$(CC) -c -o line.o line.c $(CFLAGS)

clean:
	@-rm -f $(ALL_APPLICATIONS) line.o

include ../library.mk
