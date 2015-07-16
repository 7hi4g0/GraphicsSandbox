#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Circle/circle.h>

#define	MaxPoints	32

BUTTON_RELEASE(buttonRelease);

char windowName[] = "Circle";
char fileName[] = "circle.ppm";

Point points[MaxPoints];
uint32_t totalPoints = 4;
uint32_t width, height;

void prepare() {
	points[0].x = 50;
	points[0].y = 550;

	points[1].x = 210;
	points[1].y = 50;

	points[2].x = 380;
	points[2].y = 550;

	points[3].x = 550;
	points[3].y = 50;

	buttonReleaseFn = buttonRelease;
}

void buttonRelease(XButtonEvent xbutton) {
	if (totalPoints < MaxPoints) {
		points[totalPoints++] = (Point) {xbutton.x, xbutton.y};
	}
}

void draw(Image image) {
		Point center = {300, 300};

		drawCircle(image, center, 20);
		drawCircle(image, center, 50);
		drawCircle(image, center, 80);
		drawCircle(image, center, 110);
}
