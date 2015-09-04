#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Point/point.h>
#include <ParametricCurve/parametric.h>

#define ParametricFnCount	2
#define	MaxPoints	32

KEY_RELEASE(keyRelease);
BUTTON_RELEASE(buttonRelease);
BUTTON_PRESS(buttonPress);
BUTTON_MOTION(buttonMotion);

void resetPoints();
void clearPoints();

char windowName[] = "Parametric Curves";
char fileName[] = "parametricCurve.ppm";

DrawMultipleParametricFn drawParametricList[2] = {
	drawMultipleBezier,
	drawMultipleBSpline
};

Point points[MaxPoints];
uint32_t totalPoints;
uint32_t thickness;
uint32_t drawFnIndex;
uint32_t pointMotionIndex;
DrawMultipleParametricFn drawParametricFn;

void resetPoints() {
	totalPoints = 4;

	points[0].x = 50;
	points[0].y = 550;

	points[1].x = 210;
	points[1].y = 50;

	points[2].x = 380;
	points[2].y = 550;

	points[3].x = 550;
	points[3].y = 50;
}

void clearPoints() {
	totalPoints = 0;
}

void prepare(void) {
	thickness = 1;
	drawFnIndex = 0;
	pointMotionIndex = MaxPoints;

	drawParametricFn = drawParametricList[drawFnIndex];

	resetPoints();

	keyReleaseFn = keyRelease;
	buttonReleaseFn = buttonRelease;
	buttonPressFn = buttonPress;
	buttonMotionFn = buttonMotion;
}

void keyRelease(XKeyEvent xkey) {
	switch (XLookupKeysym(&xkey, 0)) {
		case XK_KP_Add:
			if (thickness < 50) {
				thickness += 1;
			}
//			setLineThickness(thickness);
			break;
		case XK_KP_Subtract:
			if (thickness > 1) {
				thickness -= 1;
			}
//			setLineThickness(thickness);
			break;
		case XK_p:
			drawParametricFn = drawParametricList[++drawFnIndex % ParametricFnCount];
			break;
		case XK_r:
			resetPoints();
			break;
		case XK_c:
			clearPoints();
			break;
	}
}

void buttonMotion(XMotionEvent xmotion) {
	if (pointMotionIndex < MaxPoints) {
		points[pointMotionIndex].x = xmotion.x;
		points[pointMotionIndex].y = xmotion.y;
	}
}

void buttonPress(XButtonEvent xbutton) {
	uint32_t point;

	for (point = 0; point < totalPoints; point++) {
		if (inPoint(points[point], (Point) {xbutton.x, xbutton.y})) {
			pointMotionIndex = point;
			break;
		}
	}
}

void buttonRelease(XButtonEvent xbutton) {
	if (pointMotionIndex < MaxPoints) {
		pointMotionIndex = MaxPoints;
	} else if (totalPoints < MaxPoints) {
		points[totalPoints++] = (Point) {xbutton.x, xbutton.y};
	}
}

void draw(Image image) {
	drawParametricFn(image, points, totalPoints);

	for (int point = 0; point < totalPoints; point++) {
		drawPoint(image, points[point]);
	}
}
