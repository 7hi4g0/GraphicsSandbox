#include <ParametricCurve/parametric.h>

void drawBezier(Image image, Point p1, Point p2, Point p3, Point p4) {
	Point previous, next;
	float step;
	float t, t2, t3;
	int32_t bx0, bx1, bx2, bx3, by0, by1, by2, by3;

	// TODO: Avoid conversion errors, because Point is uint32_t
	bx0 = - p1.x + 3 * p2.x - 3 * p3.x + p4.x;
	bx1 = 3 * p1.x - 6 * p2.x + 3 * p3.x;
	bx2 = -3 * p1.x + 3 * p2.x;
	bx3 = p1.x;

	by0 = - p1.y + 3 * p2.y - 3 * p3.y + p4.y;
	by1 = 3 * p1.y - 6 * p2.y + 3 * p3.y;
	by2 = -3 * p1.y + 3 * p2.y;
	by3 = p1.y;

	previous = p1;
	step = 0.01f;

	for (t = step; t < 1.0f; t += step) {
		t2 = t * t;
		t3 = t2 * t;

		next.x = bx0 * t3 + bx1 * t2 + bx2 * t + bx3;
		next.y = by0 * t3 + by1 * t2 + by2 * t + by3;

		drawLine(image, previous, next);

		previous = next;
	}
}
