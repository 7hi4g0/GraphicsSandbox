#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <inttypes.h>

#include <Image/image.h>
#include <Point/point.h>

extern uint32_t width, height;

void drawLine(Image image, Point p1, Point p2);

#endif
