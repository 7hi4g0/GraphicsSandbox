#include <Point/point.h>

void drawPoint(Image image, Point point) {
	Pixel *pixels;

	pixels = (Pixel *) image.data;

	for (int y = point.y - 2; y < point.y + 2; y++) {
		for (int x = point.x - 2; x < point.x + 2; x++) {
			pixels[y * image.width + x].value = 0x00000000;
		}
	}
}
