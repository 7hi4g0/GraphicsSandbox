#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <Window/window.h>
#include <Image/image.h>
#include <Line/line.h>

#define LineFnCount	6

char windowName[] = "Antialias";
char fileName[] = "antialias.ppm";

DrawLineFn drawLineList[LineFnCount] = {
	drawLine,
	drawLineThick,
	drawLineAntialias,
	drawLineAntialiasSlow,
	drawLineAntialiasSlow2,
	drawLineAntialiasXiaolinWu
};

Point points[4];

void prepare(void) {
	points[0].x = 350;
	points[0].y = 50;

	points[1].x = 500;
	points[1].y = 200;

	points[2].x = 350;
	points[2].y = 300;

	points[3].x = 500;
	points[3].y = 550;
}

void draw(Image image) {
	int shift = 0;
	for (int drawFn = 0; drawFn < LineFnCount; drawFn++) {
		for (int point = 0; point < 3; point++) {
			Point p1, p2;

			p1 = points[point];
			p2 = points[point + 1];

			p1.x += shift;
			p2.x += shift;

			drawLineList[drawFn](image, p1, p2);
		}

		shift -= 50;
	}
}
