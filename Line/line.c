#include "line.h"

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
	int32_t x, y;
	int32_t error, e2;
	int32_t dx, dy;
	int32_t sx, sy;

	data = (uint32_t *) image.data;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;

	error = (dx > dy ? -dx : dy) / 2;

	data[p1.y * image.width + p1.x] = 0x00000000;
	data[p2.y * image.width + p2.x] = 0x00000000;

	while (1) {
		data[y * image.width + x] = 0x00000000;
		if (x == p2.x && y == p2.y) break;

		e2 = error;

		if (e2 < dx) {
			error += dy;
			x += sx;
		}
		if (e2 > -dy) {
			error -= dx;
			y += sy;
		}
	}
}

uint32_t color(uint8_t intensity) {
	return (intensity << 16) + (intensity << 8) + intensity;
}

void drawLineAntialias(Image image, Point p1, Point p2) {
	uint32_t *data;
	uint16_t error, errorLast;
	uint16_t errorAdj;
	uint8_t weight;
	int32_t x, y;
	int32_t dx, dy;
	int32_t sx, sy;

	data = (uint32_t *) image.data;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	error = 0;

	data[y * image.width + x] = 0x00000000;

	if (dy > dx) {
		errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;

		while (--dy) {
			errorLast = error;
			error += errorAdj;

			if (error < errorLast) {
				x += sx;
			}
			y += sy;

			weight = error >> 8;

			data[y * image.width + x] = color(weight);
			data[y * image.width + x + sx] = color(weight ^ 0xFF);
		}
	} else {
		errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

		while (--dx) {
			errorLast = error;
			error += errorAdj;

			if (error < errorLast) {
				y += sy;
			}
			x += sx;

			weight = error >> 8;

			data[y * image.width + x] = color(weight);
			data[(y + sy) * image.width + x] = color(weight ^ 0xFF);
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
	int32_t sx, sy;

	data = (uint32_t *) image.data;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	error = 0;

	data[y * image.width + x] = 0x00000000;

	if (dy > dx) {
		errorAdj = ((uint32_t) dx << 16) / (uint32_t) dy;

		while (--dy) {
			errorLast = error;
			error += errorAdj;

			if (error < errorLast) {
				x += sx;
			}
			y += sy;

			data[y * image.width + x] = color((uint8_t) sqrtf(error));
			data[y * image.width + x + sx] = color((uint8_t) sqrtf(error ^ 0xFFFF));
		}
	} else {
		errorAdj = ((uint32_t) dy << 16) / (uint32_t) dx;

		while (--dx) {
			errorLast = error;
			error += errorAdj;

			if (error < errorLast) {
				y += sy;
			}
			x += sx;

			data[y * image.width + x] = color((uint8_t) sqrtf(error));
			data[(y + sy) * image.width + x] = color((uint8_t) sqrtf(error ^ 0xFFFF));
		}
	}

	data[p2.y * image.width + p2.x] = 0x00000000;
}
