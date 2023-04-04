#include <stdlib.h>

#include <Point/point.h>

void drawPoint(Image image, Point point, uint8_t size) {
	Pixel *pixels;

	pixels = (Pixel *) image.data;

	// TODO: Handle edge cases, i.e. border of the image
	for (int y = point.y - size; y <= point.y + size; y++) {
		for (int x = point.x - size; x <= point.x + size; x++) {
			pixels[y * image.width + x].value = 0x00000000;
		}
	}
}

int inPoint(Point point, Point pos, uint8_t tolerance) {
	return abs(point.x - pos.x) <= tolerance && abs(point.y - pos.y) <= tolerance;
}
