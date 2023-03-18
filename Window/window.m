#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#import <Cocoa/Cocoa.h>

#include <Image/image.h>
#include <Window/window.h>

uint32_t verbose;
uint32_t debug;
uint32_t width, height;

KEY_RELEASE(keyReleaseStub) {
}

BUTTON_RELEASE(buttonReleaseStub){
}

BUTTON_PRESS(buttonPressStub){
}

BUTTON_MOTION(buttonMotionStub){
}

KeyReleaseFn *keyReleaseFn = keyReleaseStub;
ButtonReleaseFn *buttonReleaseFn = buttonReleaseStub;
ButtonPressFn *buttonPressFn = buttonPressStub;
ButtonMotionFn *buttonMotionFn = buttonMotionStub;

@interface MYWindowDelegate : NSResponder <NSWindowDelegate>

- (BOOL)windowShouldClose:(id)sender;

@end

@implementation MYWindowDelegate

- (BOOL)windowShouldClose:(id)sender
{
	printf("Window will close\n");
    return YES;
}

@end

int
main(int argc, char *argv[])
{ @autoreleasepool
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    NSWindow *nswindow;
    NSRect rect;
    NSUInteger style;
    NSArray *screens = [NSScreen screens];
    NSScreen *screen = nil;

	// Atom delete_event;
	// GC gc;
	// XImage image;

	// Default global values
	width = 600;
	height = 600;
	verbose = 0;
	debug = 0;

	{
		char opt;

		while ((opt = getopt(argc, argv, ":dv")) != -1) {
			switch (opt) {
				case 'd':
					debug +=1;
					break;
				case 'v':
					verbose += 1;
					break;
				case ':':
					fprintf(stderr, "%c needs an argument\n", optopt);
					exit(-1);
					break;
				default:
					fprintf(stderr, "Unknown option %c\n", optopt);
					exit(-1);
					break;
			}
		}
	}

	prepare();

    [NSApplication sharedApplication];

    rect.origin.x = 100;
    rect.origin.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - 100 - height;
    rect.size.width = width;
    rect.size.height = height;

    style = NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

    screen = screens[0];

    @try {
        nswindow = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:NO screen:screen];
    }
    @catch (NSException *e) {
        return fprintf(stderr, "%s", [[e reason] UTF8String]);
    }

    [nswindow setColorSpace:[NSColorSpace sRGBColorSpace]];

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 101200 /* Added in the 10.12.0 SDK. */
    /* By default, don't allow users to make our window tabbed in 10.12 or later */
    if ([nswindow respondsToSelector:@selector(setTabbingMode:)]) {
        [nswindow setTabbingMode:NSWindowTabbingModeDisallowed];
    }
#endif

    [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];

    // [nswindow setLevel:NSFloatingWindowLevel];

    [nswindow setBackgroundColor:[NSColor blueColor]];

	MYWindowDelegate *windowDelegate = [[MYWindowDelegate alloc] init];
	[nswindow setDelegate:windowDelegate];

	[nswindow setTitle:[NSString stringWithUTF8String:windowName]];

    [nswindow makeKeyAndOrderFront:NSApp];
    [nswindow display];

	[NSApp finishLaunching];

	// image.width = width;
	// image.height = height;
	// image.xoffset = 0;
	// image.format = ZPixmap;
	// image.data = NULL;
	// image.byte_order = LSBFirst;
	// image.bitmap_unit = 32;
	// image.bitmap_bit_order = MSBFirst;
	// image.bitmap_pad = 32;
	// image.depth = 24;
	// image.bytes_per_line = 0;
	// image.bits_per_pixel = 32;
	// image.red_mask = vi->red_mask;
	// image.green_mask = vi->green_mask;
	// image.blue_mask = vi->blue_mask;
	// image.obdata = NULL;

	// image.data = (char *) malloc(sizeof(char) * width * height * 4);

	// if (image.data == NULL) {
	// 	fprintf(stderr, "Unable to allocate memory\n");
	// }

	// XInitImage(&image);

	Image img;

	img.data = (char *) malloc(sizeof(char) * width * height * 4);
	img.width = width;
	img.height = height;

	FILE *imageFile;
	Pixel *pixel;
	NSEvent *event = nil;
	// XEvent xev;
	// KeySym keysym;

	char __block loop = 1;

	NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
	[center addObserverForName:NSWindowWillCloseNotification object:nil queue:[NSOperationQueue mainQueue]
		usingBlock:^(NSNotification *note) {
			printf("Received notification: %s\n", [[note name] UTF8String]);
			loop = 0;
		}
	];

	while (loop) {
		[pool release];
		pool = [[NSAutoreleasePool alloc] init];

		while ((event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES]) != nil) {
			NSEventType eventType = [event type];
			switch (eventType) {
				case NSEventTypeKeyUp: {
					unsigned short keyCode = [event keyCode];
					switch (keyCode) {
					// 	case XK_s:
					// 		imageFile = fopen(fileName, "w");

					// 		if (imageFile != NULL) {
					// 			fprintf(imageFile, "P3\n%d %d\n255\n", width, height);

					// 			pixel = (Pixel *) image.data;

					// 			for (int y = 0; y < height; y++) {
					// 				for (int x = 0; x < width; x++) {
					// 					fprintf(imageFile, "%d %d %d ", pixel->R, pixel->G, pixel->B);
					// 					pixel++;
					// 				}
					// 				fprintf(imageFile, "\n");
					// 			}

					// 			fclose(imageFile);
					// 			imageFile = NULL;

					// 			fprintf(stderr, "File saved!\n");
					// 		}
					// 		break;
						default:
							if (verbose) {
								fprintf(stderr, "Untreated '%hu' captured.\n", keyCode);
							}
							break;
					}

					KeyboardEvent keyboardEvent;
					keyReleaseFn(keyboardEvent);
				}
					break;
				case NSEventTypeLeftMouseDown:
				case NSEventTypeOtherMouseDown:
				case NSEventTypeRightMouseDown: {
					MouseEvent mouseEvent;
					buttonPressFn(mouseEvent);
				}
					break;
				case NSEventTypeLeftMouseUp:
				case NSEventTypeOtherMouseUp:
				case NSEventTypeRightMouseUp: {
					MouseEvent mouseEvent;
					buttonPressFn(mouseEvent);
				}
					break;
				case NSEventTypeLeftMouseDragged:
				case NSEventTypeRightMouseDragged:
				case NSEventTypeOtherMouseDragged: /* usually middle mouse dragged */
				case NSEventTypeMouseMoved:
				case NSEventTypeScrollWheel: {
					MotionEvent motionEvent;
					buttonMotionFn(motionEvent);
				}
					break;
				case NSEventTypeKeyDown:
				case NSEventTypeFlagsChanged:
				default:
					break;
			}

			printf("%lu\n", eventType);

			[NSApp sendEvent:event];
		}

		uint32_t *data = (uint32_t *) img.data;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				*data++ = 0x00FFFFFF;
			}
		}

		data = (uint32_t *) img.data;

		draw(img);

		// XPutImage(dpy, win, gc, &image, 0, 0, 0, 0, width, height);
		[NSApp updateWindows];
	}

	[nswindow close];
	// [NSApp terminate];

    [pool release];
}
}