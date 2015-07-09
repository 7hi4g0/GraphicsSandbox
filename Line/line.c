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

/* TODO(tandrade): Try this later
typedef union {
	Pixel sub[9];
	struct {
		Pixel dr;
		Pixel dc;
		Pixel dl;
		Pixel cr;
		Pixel cc;
		Pixel cl;
		Pixel tr;
		Pixel tc;
		Pixel tl;
	};
} Subpixel9;
*/

typedef union {
	Pixel sub[4];
	struct {
		Pixel dr;
		Pixel dl;
		Pixel tr;
		Pixel tl;
	};
} Subpixel4;

void clearSubImage(Image image) {
	Subpixel4 *subpixel;

	subpixel = (Subpixel4 *) image.data;

	for (int y = 0; y < image.height; y++) {
		for (int x = 0; x < image.width; x++) {
			subpixel->sub[0] = (Pixel) 0x00FFFFFFU;
			subpixel->sub[1] = (Pixel) 0x00FFFFFFU;
			subpixel->sub[2] = (Pixel) 0x00FFFFFFU;
			subpixel->sub[3] = (Pixel) 0x00FFFFFFU;

			subpixel++;
		}
	}
}

Pixel subPixelAntialias(Subpixel4 *subpixel) {
	Pixel blend;
	uint64_t luminance;

	luminance = (uint16_t) subpixel->dr.R * (uint16_t) subpixel->dr.R
		+ (uint16_t) subpixel->dl.R * (uint16_t) subpixel->dl.R
		+ (uint16_t) subpixel->tr.R * (uint16_t) subpixel->tr.R
		+ (uint16_t) subpixel->tl.R * (uint16_t) subpixel->tl.R;

	luminance /= 4;

	blend.R = sqrt(luminance);

	luminance = (uint16_t) subpixel->dr.G * (uint16_t) subpixel->dr.G
		+ (uint16_t) subpixel->dl.G * (uint16_t) subpixel->dl.G
		+ (uint16_t) subpixel->tr.G * (uint16_t) subpixel->tr.G
		+ (uint16_t) subpixel->tl.G * (uint16_t) subpixel->tl.G;

	luminance /= 4;

	blend.G = sqrt(luminance);

	luminance = (uint16_t) subpixel->dr.B * (uint16_t) subpixel->dr.B
		+ (uint16_t) subpixel->dl.B * (uint16_t) subpixel->dl.B
		+ (uint16_t) subpixel->tr.B * (uint16_t) subpixel->tr.B
		+ (uint16_t) subpixel->tl.B * (uint16_t) subpixel->tl.B;

	luminance /= 4;

	blend.B = sqrt(luminance);

	return blend;
}

void putSubPixel(Image image, int32_t x, int32_t y, Pixel pixel) {
	Subpixel4 *subpixel;

	subpixel = image.data + ((y / 2) * image.width + (x / 2));

	subpixel->dr = pixel;

	if ((x % 2) && (y % 2)) {
		subpixel[1].dl = pixel;
		subpixel[image.width].tr = pixel;
		subpixel[image.width + 1].tl = pixel;
	} else if (x % 2) {
		subpixel[0].tr = pixel;
		subpixel[1].dl = pixel;
		subpixel[1].dr = pixel;
	} else if (y % 2) {
		subpixel[0].dl = pixel;
		subpixel[image.width].tl = pixel;
		subpixel[image.width].tr = pixel;
	} else {
		subpixel[0].dl = pixel;
		subpixel[0].tl = pixel;
		subpixel[0].tr = pixel;
	}
}

void drawLineAntialias4(Image image, Point p1, Point p2) {
	Subpixel4 *subdata;
	Image subimg;
	uint32_t *data;
	int32_t x, y;
	int32_t error, e2;
	int32_t dx, dy;
	int32_t sx, sy;

	data = (uint32_t *) image.data;

	p1.x *= 2; p1.y *= 2;
	p2.x *= 2; p2.y *= 2;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;

	subdata = (Subpixel4 *) malloc((dy + 4) * (dx + 4) * sizeof(Pixel));

	subimg.data = subdata;
	subimg.width = dx + 2;		// 2 subpixels == 4 pixels
	subimg.height = dy + 2;		// 2 subpixels == 4 pixels

	clearSubImage(subimg);

	error = (dx > dy ? -dx : dy) / 2;

	putSubPixel(subimg, p1.x, p1.y, (Pixel) 0x00000000U);
	putSubPixel(subimg, p2.x, p2.y, (Pixel) 0x00000000U);

	while (1) {
		putSubPixel(subimg, x, y, (Pixel) 0x00000000U);
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

	p1.x /= 2; p1.y /= 2;
	p2.x /= 2; p2.y /= 2;

	dx = abs(p2.x - p1.x) + 1;
	dy = abs(p2.y - p1.y) + 1;

	x = p1.x < p2.x ? p1.x : p2.x;
	y = p1.y < p2.y ? p1.y : p2.y;

	while (dy--) {
		while (dx--) {
			data[y * image.width + x] = subPixelAntialias(&subdata[y * subimg.width + x]).value;
			x++;
		}
		y++;
	}

	free(subdata);
}
