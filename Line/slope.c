#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Line/line.h>

#define LineFnCount	4
#define	MaxPoints	32

KEY_RELEASE(keyRelease);

char windowName[] = "Slopes";
char fileName[] = "slope.ppm";

DrawLineFn drawLineList[4] = {
	drawLine,
	drawLineThick,
	drawLineAntialias,
	drawLineAntialiasSlow
};

char* drawLineNameList[4] = {
	"drawLine",
	"drawLineThick",
	"drawLineAntialias",
	"drawLineAntialiasSlow"
};

Point points[MaxPoints];
uint32_t totalPoints;
uint32_t thickness;
uint32_t drawFnIndex;
uint32_t sloped;
DrawLineFn drawLineFn;

void prepare(void) {
	totalPoints = 2;
	thickness = 1;
	drawFnIndex = 0;
	sloped = 0;
	drawLineFn = drawLineList[drawFnIndex];

	if (debug) {
		printf("%s\n", drawLineNameList[drawFnIndex]);
	}

	points[0].x = 50;
	points[0].y = 550;

	points[1].x = 50;
	points[1].y = 50;

	for (uint32_t dist = 15, point = 2; (dist + 50 < width) && (point + 1 < MaxPoints); dist *= 2, point += 2, totalPoints += 2) {
		points[point].x = 50 + dist;
		points[point].y = 550;

		points[point + 1].x = 50 + dist;
		points[point + 1].y = 50;
	}

	keyReleaseFn = keyRelease;
}

void keyRelease(KeyboardEvent keyboardEvent) {
	switch (keyboardEvent.keycode) {
		case KEYCODE_EQUALS:
			if (thickness < 50) {
				thickness += 1;
			}
			setLineThickness(thickness);
			break;
		case KEYCODE_MINUS:
			if (thickness > 1) {
				thickness -= 1;
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
		case KEYCODE_C:
			sloped = !sloped;
			break;
		default:
			break;
	}
}

void draw(Image image) {
	for (int point = sloped ? 2 : 1; point < totalPoints; point += sloped ? 2 : 1) {
		drawLineFn(image, points[point - 1], points[point]);
	}
}
