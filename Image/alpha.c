#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Window/window.h>
#include <Image/image.h>

char windowName[] = "Alpha Blending";
char fileName[] = "alpha.ppm";

uint32_t width, height;

void prepare() {
}

void draw(Image image) {
	uint32_t *data;

	data = (uint32_t *) image.data;

	for (int y = 0; y < height; y++) {
		Pixel blackAlpha = {0};

		blackAlpha.A = y;

		for (int x = 0; x < width; x++) {
			*data++ = alphaBlend(blackAlpha, (Pixel) *data).value;
		}
	}
}
