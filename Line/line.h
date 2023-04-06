#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <inttypes.h>

#include <math.h>

#include <Image/image.h>
#include <Point/point.h>

typedef void (*DrawLineFn)(Image, Point, Point);

extern void setLineThickness(uint32_t lineThickness);
extern void drawLine(Image image, Point p1, Point p2);
extern void drawLineThick(Image image, Point p1, Point p2);
extern void drawLineAntialias(Image image, Point p1, Point p2);
extern void drawLineAntialiasSlow(Image image, Point p1, Point p2);
extern void drawLineAntialiasSlow2(Image image, Point p1, Point p2);
extern void drawLineAntialiasXiaolinWu(Image image, Point p1, Point p2);

#endif
