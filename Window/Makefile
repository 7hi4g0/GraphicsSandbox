include ../defs.mk

WINDOW_SRC := window.c

ifneq ($(OS),Windows_NT)
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
		WINDOW_SRC := window.m
    endif
endif

all: main.c window.o
	$(CC) -o window main.c window.o $(CFLAGS) $(GRAPHICS_LINKER)

window.o: $(WINDOW_SRC) window.h
	$(CC) -c -o window.o $(WINDOW_SRC) $(CFLAGS)

clean:
	-rm -f window window.o
