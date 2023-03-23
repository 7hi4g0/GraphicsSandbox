CFLAGS=-std=gnu11

all:
	cd Window; make
	cd Image; make
	cd Point; make
	cd Line; make
	cd Bezier; make
	cd ParametricCurve; make
