#ifndef POINT_H
#define POINT_H

#include <Image/image.h>

#ifdef __APPLE__
#define Point MYPoint
#endif

typedef struct {
	uint16_t x;
	uint16_t y;
} Point;

void drawPoint(Image image, Point point, uint8_t size);
int inPoint(Point point, Point pos, uint8_t tolerance);

#endif
