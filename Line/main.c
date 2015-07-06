#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
	uint32_t x;
	uint32_t y;
} Point;

typedef union {
	uint32_t value;
	struct {
		uint8_t B;
		uint8_t G;
		uint8_t R;
		uint8_t pad;
	};
} Pixel;

#define	MaxPoints	32

Point points[MaxPoints];
uint32_t totalPoints = 4;
uint32_t width, height;

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
void drawLine(void *image, Point p1, Point p2) {
	uint32_t *data;
	int32_t x, y;
	int32_t error, e2;
	int32_t dx, dy;
	int32_t sx, sy;

	data = (uint32_t *) image;

	x = p1.x;
	y = p1.y;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;

	error = (dx > dy ? -dx : dy) / 2;

	data[p1.y * width + p1.x] = 0x00000000;
	data[p2.y * width + p2.x] = 0x00000000;

	while (1) {
		data[y * width + x] = 0x00000000;
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

void drawLineAntialias(void *image, Point p1, Point p2) {
	uint32_t *data;
	uint16_t error, errorLast;
	uint16_t errorAdj;
	uint8_t weight;
	int32_t x, y;
	int32_t dx, dy;
	int32_t sx, sy;

	data = (uint32_t *) image;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;

	x = p1.x;
	y = p1.y;

	error = 0;

	data[y * width + x] = 0x00000000;

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

			data[y * width + x] = color(weight);
			data[y * width + x + sx] = color(weight ^ 0xFF);
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

			data[y * width + x] = color(weight);
			data[(y + sy) * width + x] = color(weight ^ 0xFF);
		}
	}

	data[p2.y * width + p2.x] = 0x00000000;
}

int main() {
	Display *dpy;
	Visual *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	Atom delete_event;
	GC gc;
	XImage image;
	Window root;
	Window win;
	int screen;

	width = 600;
	height = 600;
	
	dpy = XOpenDisplay(NULL);

	root = DefaultRootWindow(dpy);
	screen = DefaultScreen(dpy);
	cmap = DefaultColormap(dpy, screen);
	vi = DefaultVisual(dpy, screen);
	gc = DefaultGC(dpy, screen);

	swa.background_pixel = 0xBCBCBC;
	swa.colormap = cmap;
	swa.event_mask = StructureNotifyMask | KeyReleaseMask | ButtonReleaseMask | ButtonPressMask;

	win = XCreateWindow(dpy, root, 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWColormap | CWEventMask, &swa);
	//win = XCreateSimpleWindow(dpy, root, 0, 0, 600, 600, 0, 0xBCBCBC);
	
	delete_event = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &delete_event, 1);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Line");

	image.width = width;
	image.height = height;
	image.xoffset = 0;
	image.format = ZPixmap;
	image.data = NULL;
	image.byte_order = LSBFirst;
	image.bitmap_unit = 32;
	image.bitmap_bit_order = MSBFirst;
	image.bitmap_pad = 32;
	image.depth = 24;
	image.bytes_per_line = 0;
	image.bits_per_pixel = 32;
	image.red_mask = vi->red_mask;
	image.green_mask = vi->green_mask;
	image.blue_mask = vi->blue_mask;
	image.obdata = NULL;

	image.data = (char *) malloc(sizeof(char) * width * height * 4);

	if (image.data == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
	}

	XInitImage(&image);

	points[0].x = 50;
	points[0].y = 550;

	points[1].x = 210;
	points[1].y = 50;

	points[2].x = 380;
	points[2].y = 550;

	points[3].x = 550;
	points[3].y = 50;

	void (*drawLineFn)(void *, Point, Point) = drawLine;
	FILE *imageFile;
	Pixel *pixel;
	XEvent xev;
	char loop = 1;

	while (loop) {
		while (XPending(dpy) > 0) {
			XNextEvent(dpy, &xev);

			switch (xev.type) {
				case ClientMessage:
					if (xev.xclient.data.l[0] == delete_event) {
						loop = 0;
					}
					break;
				case KeyRelease:
					switch (XLookupKeysym(&xev.xkey, 0)) {
						case XK_a:
							if (drawLineFn == drawLine) {
								drawLineFn = drawLineAntialias;
							} else {
								drawLineFn = drawLine;
							}
							break;
						case XK_s:
							imageFile = fopen("lines.ppm", "w");

							if (imageFile != NULL) {
								fprintf(imageFile, "P3\n%d %d\n255\n", width, height);

								pixel = (Pixel *) image.data;

								for (int y = 0; y < height; y++) {
									for (int x = 0; x < width; x++) {
										fprintf(imageFile, "%d %d %d ", pixel->R, pixel->G, pixel->B);
										pixel++;
									}
									fprintf(imageFile, "\n");
								}

								fclose(imageFile);
								imageFile = NULL;

								fprintf(stderr, "File saved!\n");
							}
							break;
						default:
							break;
					}
					break;
				case ButtonRelease:
					if (totalPoints < MaxPoints) {
						points[totalPoints++] = (Point) {xev.xbutton.x, xev.xbutton.y};
					}
					break;
				default:
					break;
			}
		}

		uint32_t *data = (uint32_t *) image.data;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				*data++ = 0x00FFFFFF;
			}
		}

		data = (uint32_t *) image.data;

		for (int point = 1; point < totalPoints; point++) {
			drawLineFn(data, points[point - 1], points[point]);
		}

		XPutImage(dpy, win, gc, &image, 0, 0, 0, 0, width, height);
	}

	free(image.data);

	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}
