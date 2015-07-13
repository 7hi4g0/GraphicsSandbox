#include <Image/image.h>
#include <Point/point.h>
#include <Circle/circle.h>

void circlePoints(Image image, Point center, Point p) {
	uint32_t *data;

	data = (uint32_t *) image.data;

	data[(center.y + p.y) * image.width + center.x + p.x] = 
	data[(center.y + p.y) * image.width + center.x - p.x] = 
	data[(center.y - p.y) * image.width + center.x + p.x] = 
	data[(center.y - p.y) * image.width + center.x - p.x] = 
		0x00000000;

	if (p.x != p.y) {
		data[(center.y + p.x) * image.width + center.x + p.y] = 
		data[(center.y + p.x) * image.width + center.x - p.y] = 
		data[(center.y - p.x) * image.width + center.x + p.y] = 
		data[(center.y - p.x) * image.width + center.x - p.y] = 
			0x00000000;
	}
}

void drawCircle(Image image, Point center, uint32_t radius) {
	Point perimeterPoint;
	int32_t deltaEast, deltaSouthEast;
	int32_t d;

	perimeterPoint.x = 0;
	perimeterPoint.y = radius;
	deltaEast = 3;
	deltaSouthEast = -2 * radius + 5;
	d = 1 - radius;
	circlePoints(image, center, perimeterPoint);

	while (perimeterPoint.y > perimeterPoint.x) {
		if (d < 0) {
			d += deltaEast;
			deltaEast += 2;
			deltaSouthEast += 2;
		} else {
			d += deltaSouthEast;
			deltaEast += 2;
			deltaSouthEast += 4;
			perimeterPoint.y--;
		}

		perimeterPoint.x++;
		circlePoints(image, center, perimeterPoint);
	}
}
