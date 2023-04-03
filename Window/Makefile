include ../defs.mk

APPLICATION := window

WINDOW_SRC := window.c

ifneq ($(OS),Windows_NT)
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
		WINDOW_SRC := window.m
    endif
endif

all: $(APPLICATION)

$(APPLICATION): main.c $(LIBRARY)
	@echo "Building $(APPLICATION) example application"
	@$(CC) $(CFLAGS) main.c -o $(APPLICATION) $(GRAPHICS_LINKER) -L$(LIBDIR) -lgraphics

window.o: $(WINDOW_SRC) window.h
	@$(CC) -c -o window.o $(WINDOW_SRC) $(CFLAGS)

clean:
	@-rm -f window window.o

.PHONY: clean $(LIBRARY)

include ../library.mk
