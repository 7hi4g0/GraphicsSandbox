#ifndef _PARAMETRIC_H_
#define _PARAMETRIC_H_

#include <Image/image.h>
#include <Point/point.h>
#include <Line/line.h>

typedef void (*DrawParametricFn)(Image image, Point p1, Point p2, Point p3, Point p4);
typedef void (*DrawMultipleParametricFn)(Image image, Point *points, int pointCount);

// Simple Curve
void drawBezier(Image image, Point p1, Point p2, Point p3, Point p4);
void drawBSpline(Image image, Point p1, Point p2, Point p3, Point p4);
// Multiple Curves
void drawMultipleBezier(Image image, Point *points, int pointCount);
void drawMultipleBSpline(Image image, Point *points, int pointCount);

#endif
