#ifndef POINT_H
#define POINT_H

#include <Image/image.h>

typedef struct {
	uint32_t x;
	uint32_t y;
} Point;

void drawPoint(Image image, Point point);

#endif
