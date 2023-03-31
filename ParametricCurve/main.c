#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

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
uint32_t highlightedPoint;
uint32_t selectedPoint;
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
	highlightedPoint = MaxPoints;
	selectedPoint = MaxPoints;

	drawParametricFn = drawParametricList[drawFnIndex];

	resetPoints();

	keyReleaseFn = keyRelease;
	buttonReleaseFn = buttonRelease;
	buttonPressFn = buttonPress;
	buttonMotionFn = buttonMotion;
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
		case KEYCODE_R:
			resetPoints();
			break;
		case KEYCODE_C:
			clearPoints();
			break;
	}
}

void buttonMotion(MouseEvent mouseEvent) {
	if (selectedPoint < MaxPoints) {
		points[selectedPoint] = mouseEvent.location;
		return;
	}

	highlightedPoint = MaxPoints;
	for (uint32_t point = 0; point < totalPoints; point++) {
		if (inPoint(points[point], mouseEvent.location, 3)) {
			highlightedPoint = point;
			break;
		}
	}
}

void buttonPress(MouseEvent mouseEvent) {
	if (highlightedPoint < MaxPoints) {
		selectedPoint = highlightedPoint;
	}
}

void buttonRelease(MouseEvent mouseEvent) {
	if (selectedPoint < MaxPoints) {
		selectedPoint = MaxPoints;
	} else if (totalPoints < MaxPoints) {
		points[totalPoints++] = mouseEvent.location;
	}
}

void draw(Image image) {
	drawParametricFn(image, points, totalPoints);

	for (int point = 0; point < totalPoints; point++) {
		uint8_t pointSize = (point == highlightedPoint) ? 3 : 2;
		drawPoint(image, points[point], pointSize);
		// TODO: remove debug check and change line to be dashed or of a different color
		if (debug && point > 0) {
			drawLine(image, points[point - 1], points[point]);
		}
	}
}
