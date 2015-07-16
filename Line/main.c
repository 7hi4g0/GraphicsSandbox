#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Line/line.h>

#define LineFnCount	4
#define	MaxPoints	32

KEY_RELEASE(keyRelease);
BUTTON_RELEASE(buttonRelease);

char windowName[] = "Lines";
char fileName[] = "lines.ppm";

DrawLineFn drawLineList[4] = {
	drawLine,
	drawLineThick,
	drawLineAntialias,
	drawLineAntialiasSlow
};

Point points[MaxPoints];
uint32_t totalPoints = 4;
uint32_t thickness = 1;
uint32_t drawFnIndex = 0;
DrawLineFn drawLineFn;

void prepare(void) {
	drawLineFn = drawLineList[drawFnIndex];

	points[0].x = 50;
	points[0].y = 550;

	points[1].x = 210;
	points[1].y = 50;

	points[2].x = 380;
	points[2].y = 550;

	points[3].x = 550;
	points[3].y = 50;

	keyReleaseFn = keyRelease;
	buttonReleaseFn = buttonRelease;
}

void keyRelease(XKeyEvent xkey) {
	switch (XLookupKeysym(&xkey, 0)) {
		case XK_KP_Add:
			if (thickness < 50) {
				thickness += 1;
			}
			setLineThickness(thickness);
			break;
		case XK_KP_Subtract:
			if (thickness > 1) {
				thickness -= 1;
			}
			setLineThickness(thickness);
			break;
		case XK_a:
			drawLineFn = drawLineList[++drawFnIndex % LineFnCount];
			break;
	}
}

void buttonRelease(XButtonEvent xbutton) {
	if (totalPoints < MaxPoints) {
		points[totalPoints++] = (Point) {xbutton.x, xbutton.y};
	}
}

void draw(Image image) {
	for (int point = 1; point < totalPoints; point++) {
		drawLineFn(image, points[point - 1], points[point]);
	}
}
