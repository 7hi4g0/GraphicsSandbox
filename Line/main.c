#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Line/line.h>

#define LineFnCount	5
#define	MaxPoints	32

KEY_RELEASE(keyRelease);
BUTTON_PRESS(buttonPress);
BUTTON_RELEASE(buttonRelease);
BUTTON_MOTION(buttonMotion);

char windowName[] = "Lines";
char fileName[] = "lines.ppm";

DrawLineFn drawLineList[LineFnCount] = {
	drawLine,
	drawLineThick,
	drawLineAntialias,
	drawLineAntialiasSlow,
	drawLineAntialiasXiaolinWu
};

char* drawLineNameList[LineFnCount] = {
	"drawLine",
	"drawLineThick",
	"drawLineAntialias",
	"drawLineAntialiasSlow",
	"drawLineAntialiasXiaolinWu"
};

Point points[MaxPoints];
uint32_t totalPoints;
uint32_t thickness;
uint32_t drawFnIndex;
uint32_t showPoints;
uint32_t basePointSize;
uint32_t highlightPointSize;
uint32_t highlightedPoint;
uint32_t selectedPoint;
DrawLineFn drawLineFn;

void prepare(void) {
	totalPoints = 4;
	thickness = 1;
	drawFnIndex = 0;
	showPoints = 0;
	basePointSize = 2;
	highlightPointSize = 3;
	highlightedPoint = MaxPoints;
	selectedPoint = MaxPoints;
	drawLineFn = drawLineList[drawFnIndex];

	if (debug) {
		printf("%s\n", drawLineNameList[drawFnIndex]);
	}

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
	buttonPressFn = buttonPress;
	buttonMotionFn = buttonMotion;
}

void keyRelease(KeyboardEvent keyboardEvent) {
	switch (keyboardEvent.keycode) {
		case KEYCODE_EQUALS:
			if (thickness < 50) {
				thickness += 1;
			}
			if (basePointSize < 25) {
				uint8_t highlightIncrement;
				basePointSize +=1;
				highlightIncrement = (basePointSize >> 2);
				highlightPointSize = basePointSize + (highlightIncrement > 0 ? highlightIncrement : 1);
				if (debug) {
					printf("point size: %d %d\n", basePointSize, highlightPointSize);
				}
			}
			setLineThickness(thickness);
			break;
		case KEYCODE_MINUS:
			if (thickness > 1) {
				thickness -= 1;
			}
			if (basePointSize > 0) {
				uint8_t highlightIncrement;
				basePointSize -=1;
				highlightIncrement = (basePointSize >> 2);
				highlightPointSize = basePointSize + (highlightIncrement > 0 ? highlightIncrement : 1);
				if (debug) {
					printf("point size: %d %d\n", basePointSize, highlightPointSize);
				}
			}
			setLineThickness(thickness);
			break;
		case KEYCODE_A:
			drawFnIndex++;
			drawFnIndex %= LineFnCount;
			drawLineFn = drawLineList[drawFnIndex];
			if (debug) {
				printf("%s\n", drawLineNameList[drawFnIndex]);
			}
			break;
		case KEYCODE_P:
			showPoints = !showPoints;
			break;
		case KEYCODE_C:
			totalPoints = 0;
			break;
		default:
			break;
	}
}

void buttonMotion(MouseEvent mouseEvent) {
	if (!showPoints) {
		return;
	}

	if (selectedPoint < MaxPoints) {
		points[selectedPoint] = mouseEvent.location;
		return;
	}

	highlightedPoint = MaxPoints;
	for (uint32_t point = 0; point < totalPoints; point++) {
		if (inPoint(points[point], mouseEvent.location, highlightPointSize)) {
			highlightedPoint = point;
			break;
		}
	}
}

void buttonPress(MouseEvent mouseEvent) {
	if (!showPoints) {
		return;
	}

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
	uint8_t pointSize;
	if (showPoints) {
		pointSize = (0 == highlightedPoint) ? highlightPointSize : basePointSize;
		drawPoint(image, points[0], pointSize);
	}
	for (int point = 1; point < totalPoints; point++) {
		if (showPoints) {
			pointSize = (point == highlightedPoint) ? highlightPointSize : basePointSize;
			drawPoint(image, points[point], pointSize);
		}
		drawLineFn(image, points[point - 1], points[point]);
	}
}
