#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <inttypes.h>

#include <Image/image.h>

typedef struct {
	uint32_t x;
	uint32_t y;
} Point;

typedef union {
	uint32_t value;
	struct {
		uint8_t B;
		uint8_t G;
		uint8_t R;
		uint8_t pad;
	};
} Pixel;

extern uint32_t width, height;

void drawLine(Image image, Point p1, Point p2);

#endif
