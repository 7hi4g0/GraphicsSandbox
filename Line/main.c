#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Image/image.h>
#include <Line/line.h>

#define	MaxPoints	32

Point points[MaxPoints];
uint32_t totalPoints = 4;
uint32_t width, height;

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

	Image img;

	img.data = image.data;
	img.width = width;
	img.height = height;

	void (*drawLineFn)(Image, Point, Point) = drawLine;
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
							} else if (drawLineFn == drawLineAntialias) {
								drawLineFn = drawLineAntialias4;
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
			drawLineFn(img, points[point - 1], points[point]);
		}

		XPutImage(dpy, win, gc, &image, 0, 0, 0, 0, width, height);
	}

	free(image.data);

	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}
