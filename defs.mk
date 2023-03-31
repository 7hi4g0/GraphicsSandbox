SUBPROJECTS+=Window/
SUBPROJECTS+=Image/
SUBPROJECTS+=Line/
SUBPROJECTS+=Point/
SUBPROJECTS+=Circle/
SUBPROJECTS+=Bezier/
SUBPROJECTS+=ParametricCurve/

CFLAGS=-std=gnu11 -I$(dir $(abspath $(lastword $(MAKEFILE_LIST))))

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