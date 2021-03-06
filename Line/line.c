#include <Line/line.h>

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
	int32_t sx, sy;
	int32_t addrx, addry;

	data = (uint32_t *) image.data;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;
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
			x += sx;
			addr += addrx;
		}
		if (e2 > -dy) {
			error -= dx;
			y += sy;
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
	int32_t sx, sy;
	int32_t addrx, addry;
	int32_t startThick, thickAdd;

	data = (uint32_t *) image.data;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;
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
			x += sx;
			data += addrx;
		}
		if (e2 > -dy) {
			error -= dx;
			y += sy;
			data += addry;
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
