#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Circle/circle.h>

char windowName[] = "Circle";
char fileName[] = "circle.ppm";

uint32_t width, height;

void prepare() {
}

void draw(Image image) {
		Point center = {300, 300};

		drawCircle(image, center, 20);
		drawCircle(image, center, 50);
		drawCircle(image, center, 80);
		drawCircle(image, center, 110);
}
