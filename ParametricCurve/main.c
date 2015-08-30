#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Window/window.h>
#include <Image/image.h>
#include <ParametricCurve/parametric.h>

#define ParametricFnCount	1
#define	MaxPoints	32

KEY_RELEASE(keyRelease);
BUTTON_RELEASE(buttonRelease);

char windowName[] = "Parametric Curves";
char fileName[] = "parametricCurve.ppm";

DrawParametricFn drawParametricList[4] = {
	drawBezier,
	//drawBSpline
};

Point points[MaxPoints];
uint32_t totalPoints = 4;
uint32_t thickness = 1;
uint32_t drawFnIndex = 0;
DrawParametricFn drawParametricFn;

void prepare(void) {
	drawParametricFn = drawParametricList[drawFnIndex];

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
//			setLineThickness(thickness);
			break;
		case XK_KP_Subtract:
			if (thickness > 1) {
				thickness -= 1;
			}
//			setLineThickness(thickness);
			break;
		case XK_a:
			drawParametricFn = drawParametricList[++drawFnIndex % ParametricFnCount];
			break;
	}
}

void buttonRelease(XButtonEvent xbutton) {
	if (totalPoints < MaxPoints) {
		points[totalPoints++] = (Point) {xbutton.x, xbutton.y};
	}
}

void draw(Image image) {
	drawParametricFn(image, points[0], points[1], points[2], points[3]);
}
