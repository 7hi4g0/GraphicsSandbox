#include <Line/line.h>
#include <Image/image.h>

static uint32_t sLineThickness = 1;

// Bresenham's algorithm
// A = dy
// B = -dx
// C = dx * b
// f(x,y) = A * x + B * y + C
// If f(x,y) = 0, (x,y) is part of the line
// If f(x,y) < 0, (x,y) is below the line
// If f(x,y) > 0, (x,y) is above the line
//
// y\x 01234
//  0  \++++
//  1  -\+++
//  2  --\++
//  3  ---\+
//  4  ----\
//
// Calculate the accumulated error to determine the next pixel.
// Begin at y + 1/2 for greater dx, or at x + 1/2 for greater dy.
// E = f(x + sx,y + sy) - f(x,y)
// E = A * sx + B * sy
// If E < 0, (x + sx,y + sy) is below the line.
// If E > 0, (x + sx,y + sy) is above the line.
// For x + 1: E = E + A
// For y + 1: E = E + B
//
// Increment the pixel values.
// If y + 1 is below the line, increment x.
// If x + 1 is above the line, increment y.
// E + B < 0 <=> E < -B
// E + A > 0 <=> E > -A
//
void drawLine(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint32_t addr;
	int32_t x, y;
	int32_t error, e2;
	int32_t dx, dy;
	int32_t xStep, yStep;
	int32_t addrx, addry;

	data = (uint32_t *) image.data;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	xStep = p1.x < p2.x ? 1 : -1;
	yStep = p1.y < p2.y ? 1 : -1;
	addrx = p1.x < p2.x ? 1 : -1;
	addry = p1.y < p2.y ? image.width : -image.width;

	error = (dx > dy ? -dx : dy) / 2;

	data[p1.y * image.width + p1.x] = 0x00000000;
	data[p2.y * image.width + p2.x] = 0x00000000;

	addr = y * image.width + x;

	while (1) {
		data[addr] = 0x00000000;
		if (x == p2.x && y == p2.y) break;

		e2 = error;

		if (e2 < dx) {
			error += dy;
			x += xStep;
			addr += addrx;
		}
		if (e2 > -dy) {
			error -= dx;
			y += yStep;
			addr += addry;
		}
	}
}

void setLineThickness(uint32_t lineThickness) {
	sLineThickness = lineThickness;
}

void drawThickPoint(uint32_t *addr, int32_t startThick, int32_t thickAdd) {
	for (uint32_t *thickAddr = addr - startThick, thick = 0; thick < sLineThickness; thick++, thickAddr += thickAdd) {
		*thickAddr = 0x00000000;
	}
}

void drawLineThick(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint32_t x, y;
	int32_t error, e2;
	int32_t dx, dy;
	int32_t xStep, yStep;
	int32_t addrx, addry;
	int32_t startThick, thickAdd;

	data = (uint32_t *) image.data;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	xStep = p1.x < p2.x ? 1 : -1;
	yStep = p1.y < p2.y ? 1 : -1;
	addrx = p1.x < p2.x ? 1 : -1;
	addry = p1.y < p2.y ? image.width : -image.width;
	thickAdd = dx > dy ? addry : addrx;
	startThick = (sLineThickness / 2) * thickAdd;

	error = (dx > dy ? -dx : dy) / 2;

	drawThickPoint(data + p1.y * image.width + p1.x, startThick, thickAdd);
	drawThickPoint(data + p2.y * image.width + p2.x, startThick, thickAdd);

	data += y * image.width + x;

	while (1) {
		drawThickPoint(data, startThick, thickAdd);
		if (x == p2.x && y == p2.y) break;

		e2 = error;

		if (e2 < dx) {
			error += dy;
			x += xStep;
			data += addrx;
		}
		if (e2 > -dy) {
			error -= dx;
			y += yStep;
			data += addry;
		}
	}
}

Pixel color(uint8_t intensity) {
	Pixel color = {0x00000000};
	color.A = intensity;
	return color;
}

void drawPixelAlpha(uint32_t *pixel, Pixel pixelData) {
	*pixel = alphaBlend(pixelData, (Pixel) *pixel).value;
}

void drawLineAntialias(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint16_t error, errorLast;
	uint16_t errorAdj;
	uint8_t weight;
	int32_t x, y;
	int32_t dx, dy;
	int32_t xStep, yStep;

	data = (uint32_t *) image.data;

	if (p1.x > p2.x) {
		Point temp = p1;
		p1 = p2;
		p2 = temp;
	}

	dx = p2.x - p1.x; // No need for abs(). p2.x is greater or equal to p1.x.
	dy = abs(p2.y - p1.y);
	xStep = 1;
	yStep = p1.y <= p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	error = 0;

	data[y * image.width + x] = 0x00000000;

	if (dy > dx) {
		errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;

		while (--dy) {
			errorLast = error;
			error += errorAdj;

			if (error <= errorLast) {
				x += xStep;
			}
			y += yStep;

			weight = error >> 8;

			drawPixelAlpha(&data[y * image.width + x], color(weight ^ 0xFF));
			drawPixelAlpha(&data[y * image.width + x + xStep], color(weight));
		}
	} else {
		errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

		while (--dx) {
			errorLast = error;
			error += errorAdj;

			if (error <= errorLast) {
				y += yStep;
			}
			x += xStep;

			weight = error >> 8;

			drawPixelAlpha(&data[y * image.width + x], color(weight ^ 0xFF));
			drawPixelAlpha(&data[(y + yStep) * image.width + x], color(weight));
		}
	}

	data[p2.y * image.width + p2.x] = 0x00000000;
}

void drawLineAntialiasSlow(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint16_t error, errorLast;
	uint16_t errorAdj;
	int32_t x, y;
	int32_t dx, dy;
	int32_t xStep, yStep;

	data = (uint32_t *) image.data;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	xStep = p1.x < p2.x ? 1 : -1;
	yStep = p1.y < p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	error = 0;

	data[y * image.width + x] = 0x00000000;

	if (dy > dx) {
		errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;

		while (--dy) {
			errorLast = error;
			error += errorAdj;

			if (error <= errorLast) {
				x += xStep;
			}
			y += yStep;

			drawPixelAlpha(&data[y * image.width + x], color((uint8_t) sqrtf(error ^ 0xFFFF)));
			drawPixelAlpha(&data[y * image.width + x + xStep], color((uint8_t) sqrtf(error)));
		}
	} else {
		errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

		while (--dx) {
			errorLast = error;
			error += errorAdj;

			if (error <= errorLast) {
				y += yStep;
			}
			x += xStep;

			drawPixelAlpha(&data[y * image.width + x], color((uint8_t) sqrtf(error ^ 0xFFFF)));
			drawPixelAlpha(&data[(y + yStep) * image.width + x], color((uint8_t) sqrtf(error)));
		}
	}

	data[p2.y * image.width + p2.x] = 0x00000000;
}

void drawLineAntialiasSlow2(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint16_t error, errorLast;
	uint16_t errorAdj;
	int32_t x, y;
	int32_t dx, dy;
	int32_t xStep, yStep;

	data = (uint32_t *) image.data;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	xStep = p1.x < p2.x ? 1 : -1;
	yStep = p1.y < p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	error = 0;

	data[y * image.width + x] = 0x00000000;

	if (dy > dx) {
		errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;

		while (--dy) {
			errorLast = error;
			error += errorAdj;

			if (error <= errorLast) {
				x += xStep;
			}
			y += yStep;

			drawPixelAlpha(&data[y * image.width + x], color(((uint8_t) sqrtf(error)) ^ 0xFF));
			drawPixelAlpha(&data[y * image.width + x + xStep], color((uint8_t) sqrtf(error)));
		}
	} else {
		errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

		while (--dx) {
			errorLast = error;
			error += errorAdj;

			if (error <= errorLast) {
				y += yStep;
			}
			x += xStep;

			drawPixelAlpha(&data[y * image.width + x], color(((uint8_t) sqrtf(error)) ^ 0xFF));
			drawPixelAlpha(&data[(y + yStep) * image.width + x], color((uint8_t) sqrtf(error)));
		}
	}

	data[p2.y * image.width + p2.x] = 0x00000000;
}

void drawLineAntialiasXiaolinWu(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint8_t error;
	double x, y;
	int32_t dx, dy;
	int32_t xStep, yStep;
	double gradient;

	data = (uint32_t *) image.data;

	if (p1.x > p2.x) {
		Point temp = p1;
		p1 = p2;
		p2 = temp;
	}

	dx = p2.x - p1.x; // No need for abs(). p2.x is greater or equal to p1.x.
	dy = abs(p2.y - p1.y);
	xStep = 1;
	yStep = p1.y <= p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	data[((uint32_t)y) * image.width + ((uint32_t)x)] = 0x00000000;

	if (dy > dx) {
		gradient = dx / (double) dy;

		while (--dy) {
			x += gradient;
			y += yStep;

			error = (x - ((uint32_t) x)) * 0xFF;

			drawPixelAlpha(&data[((uint32_t)y) * image.width + (uint32_t)x], color(error ^ 0xFF));
			drawPixelAlpha(&data[((uint32_t)y) * image.width + (uint32_t)x + xStep], color(error));
		}
	} else {
		gradient = dy / (double) dx;
		gradient *= yStep;

		while (--dx) {
			y += gradient;
			x += xStep;

			error = (y - ((uint32_t) y)) * 0xFF;

			drawPixelAlpha(&data[((uint32_t)y) * image.width + ((uint32_t)x)], color(error ^ 0xFF));
			drawPixelAlpha(&data[((uint32_t)y + 1) * image.width + ((uint32_t)x)], color(error));
		}
	}

	data[p2.y * image.width + p2.x] = 0x00000000;
}
