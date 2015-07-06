#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <GL/glcorearb.h>

#include <Line/line.h>

Point points[4];

Point bezier(float t) {
	Point point;

	float t3, t2;

	t2 = t * t;
	t3 = t2 * t;

	float b0, b1, b2, b3;

	b0 = -t3 + 3 * t2 - 3 * t + 1;
	b1 = 3 * t3 - 6 * t2 + 3 * t;
	b2 = -3 * t3 + 3 * t2;
	b3 = t3;

	point.x = b0 * points[0].x + b1 * points[1].x + b2 * points[2].x + b3 * points[3].x;
	point.y = b0 * points[0].y + b1 * points[1].y + b2 * points[2].y + b3 * points[3].y;

	return point;
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
	uint32_t width, height;

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
	swa.event_mask = StructureNotifyMask;

	win = XCreateWindow(dpy, root, 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWColormap | CWEventMask, &swa);
	//win = XCreateSimpleWindow(dpy, root, 0, 0, 600, 600, 0, 0xBCBCBC);
	
	delete_event = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &delete_event, 1);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Bezier");

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

	Image img;

	img.data = image.data;
	img.width = width;
	img.height = height;

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

		float step = 0.01f;
		Point previous = bezier(0);

		for (float t = step; t < 1.0f; t += step) {
			Point p = bezier(t);

			drawLine(img, previous, p);

			previous = p;
		}

		XPutImage(dpy, win, gc, &image, 0, 0, 0, 0, width, height);
	}

	free(image.data);

	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}
