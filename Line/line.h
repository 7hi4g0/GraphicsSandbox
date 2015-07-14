#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <inttypes.h>

#include <math.h>

#include <Image/image.h>
#include <Point/point.h>

extern uint32_t width, height;

extern void drawLine(Image image, Point p1, Point p2, uint32_t thickness);
extern void drawLineAntialias(Image image, Point p1, Point p2, uint32_t thickness);
extern void drawLineAntialiasSlow(Image image, Point p1, Point p2, uint32_t thickness);

#endif
