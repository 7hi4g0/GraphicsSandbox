#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <Window/window.h>
#include <Image/image.h>
#include <ParametricCurve/parametric.h>

#define ParametricFnCount	2
#define	MaxPoints	32

KEY_RELEASE(keyRelease);
BUTTON_RELEASE(buttonRelease);

char windowName[] = "Parametric Curves";
char fileName[] = "parametricCurve.ppm";

DrawParametricFn drawParametricList[2] = {
	drawBezier,
	drawBSpline
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

void keyRelease(KeyboardEvent keyboardEvent) {
	switch (keyboardEvent.keycode) {
		case KEYCODE_EQUALS:
			if (thickness < 50) {
				thickness += 1;
			}
//			setLineThickness(thickness);
			break;
		case KEYCODE_MINUS:
			if (thickness > 1) {
				thickness -= 1;
			}
//			setLineThickness(thickness);
			break;
		case KEYCODE_P:
			drawParametricFn = drawParametricList[++drawFnIndex % ParametricFnCount];
			break;
	}
}

void buttonRelease(MouseEvent mouseEvent) {
	if (totalPoints < MaxPoints) {
		points[totalPoints++] = (Point) {mouseEvent.location.x, mouseEvent.location.y};
	}
}

void draw(Image image) {
	drawParametricFn(image, points[0], points[1], points[2], points[3]);
}
