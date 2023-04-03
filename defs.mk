SUBPROJECTS+=Window/
SUBPROJECTS+=Image/
SUBPROJECTS+=Line/
SUBPROJECTS+=Point/
SUBPROJECTS+=Circle/
SUBPROJECTS+=Bezier/
SUBPROJECTS+=ParametricCurve/

OBJECTS+=Window/window.o
OBJECTS+=Image/image.o
OBJECTS+=Point/point.o
OBJECTS+=Line/line.o
OBJECTS+=Circle/circle.o
OBJECTS+=ParametricCurve/parametric.o

WORKDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BUILDDIR := $(WORKDIR)build
LIBDIR := $(BUILDDIR)/lib
LIBRARY := $(LIBDIR)/libgraphics.a

CFLAGS=-std=gnu11 -I$(WORKDIR)

AR := ar

ifeq ($(OS),Windows_NT)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CC := gcc
        GRAPHICS_LINKER += -lX11
    endif
    ifeq ($(UNAME_S),Darwin)
		CC := clang
        GRAPHICS_LINKER += -framework Cocoa
    endif
endif