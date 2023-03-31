#include <stdlib.h>
#include <stdio.h>

#include <Window/window.h>
#include <Line/line.h>

char windowName[] = "Bezier";
char fileName[] = "bezier.ppm";

Point points[4];
uint32_t width, height;

Point bezier(float t) {
	Point point;

	float t3, t2;

	t2 = t * t;
	t3 = t2 * t;

	float b0, b1, b2, b3;

	b0 = -t3 + 3 * t2 - 3 * t + 1;
	b1 = 3 * t3 - 6 * t2 + 3 * t;
	b2 = -3 * t3 + 3 * t2;
	b3 = t3;

	point.x = b0 * points[0].x + b1 * points[1].x + b2 * points[2].x + b3 * points[3].x;
	point.y = b0 * points[0].y + b1 * points[1].y + b2 * points[2].y + b3 * points[3].y;

	return point;
}


void prepare() {
	points[0].x = 50;
	points[0].y = 550;

	points[1].x = 210;
	points[1].y = 50;

	points[2].x = 380;
	points[2].y = 550;

	points[3].x = 550;
	points[3].y = 50;
}

void draw(Image image) {
	float step = 0.01f;
	Point previous = bezier(0);

	for (float t = step; t < 1.0f; t += step) {
		Point p = bezier(t);

		drawLine(image, previous, p);

		previous = p;
	}
}
