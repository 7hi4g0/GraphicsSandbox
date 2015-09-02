#ifndef POINT_H
#define POINT_H

#include <Image/image.h>

typedef struct {
	uint16_t x;
	uint16_t y;
} Point;

void drawPoint(Image image, Point point);

#endif
