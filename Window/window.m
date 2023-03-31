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

@interface MYWindow : NSWindow
@end

@implementation MYWindow

// Prevents beep from key presses
- (void)keyDown:(NSEvent *)theEvent{}

@end

int
main(int argc, char *argv[])
{ @autoreleasepool
{
    NSWindow *nswindow;
    NSRect rect;
    NSUInteger style;
    NSArray *screens = [NSScreen screens];
    NSScreen *screen = nil;

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
	// This is necessary to activate the Application, which in turn is necessary to capture keyboard events
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    rect.origin.x = 100;
    rect.origin.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - 100 - height;
    rect.size.width = width;
    rect.size.height = height;

    style = NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

    screen = screens[0];

    @try {
        nswindow = [[MYWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:NO screen:screen];
    }
    @catch (NSException *e) {
        return fprintf(stderr, "%s", [[e reason] UTF8String]);
    }

    [nswindow setColorSpace:[NSColorSpace sRGBColorSpace]];

	[nswindow setTitle:[NSString stringWithUTF8String:windowName]];

    [nswindow makeKeyAndOrderFront:nil];
    nswindow.contentView.layerContentsPlacement = NSViewLayerContentsPlacementScaleProportionallyToFit;
    nswindow.contentView.wantsLayer = YES;

	[NSApp activateIgnoringOtherApps:YES];
	[NSApp finishLaunching];

	size_t bitsPerComponent = 8;
	size_t bitsPerPixel = bitsPerComponent * 4;
	size_t bytesPerRow = width * 4;
	size_t BitmapMemorySize = (width * height * 4);
	
	CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();

	Image img;

	img.width = width;
	img.height = height;
	img.data = (char *) malloc(sizeof(char) * width * height * 4);

	if (img.data == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
	}

	FILE *imageFile;
	Pixel *pixel;
	NSEvent *event = nil;

	char __block loop = 1;

	NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
	[center addObserverForName:NSWindowWillCloseNotification object:nil queue:[NSOperationQueue mainQueue]
		usingBlock:^(NSNotification *note) {
			loop = 0;
		}
	];

	while (loop)
	{ @autoreleasepool
	{
		while ((event = [nswindow nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES]) != nil) {
			NSEventType eventType = [event type];
			switch (eventType) {
				case NSEventTypeKeyUp: {
					unsigned short keyCode = [event keyCode];
					switch (keyCode) {
						case KEYCODE_S:
							imageFile = fopen(fileName, "w");

							if (imageFile != NULL) {
								fprintf(imageFile, "P3\n%d %d\n255\n", width, height);

								pixel = (Pixel *) img.data;

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
							if (verbose) {
								fprintf(stderr, "Untreated '%hu' captured.\n", keyCode);
							}
							break;
					}

					KeyboardEvent keyboardEvent = {
						keyCode
					};

					keyReleaseFn(keyboardEvent);
				}
					break;
				case NSEventTypeLeftMouseDown:
				case NSEventTypeOtherMouseDown:
				case NSEventTypeRightMouseDown: {
					MouseEvent mouseEvent;
					NSPoint windowPoint;
					windowPoint = [nswindow mouseLocationOutsideOfEventStream];
					mouseEvent.location.x = windowPoint.x;
					mouseEvent.location.y = height - windowPoint.y;
					buttonPressFn(mouseEvent);
				}
					break;
				case NSEventTypeLeftMouseUp:
				case NSEventTypeOtherMouseUp:
				case NSEventTypeRightMouseUp: {
					MouseEvent mouseEvent;
					NSPoint windowPoint;
					windowPoint = [nswindow mouseLocationOutsideOfEventStream];
					mouseEvent.location.x = windowPoint.x;
					mouseEvent.location.y = height - windowPoint.y;
					buttonReleaseFn(mouseEvent);
				}
					break;
				case NSEventTypeLeftMouseDragged:
				case NSEventTypeRightMouseDragged:
				case NSEventTypeOtherMouseDragged: /* usually middle mouse dragged */
				case NSEventTypeMouseMoved:
				case NSEventTypeScrollWheel: {
					MouseEvent mouseEvent;
					NSPoint windowPoint;
					windowPoint = [nswindow mouseLocationOutsideOfEventStream];
					mouseEvent.location.x = windowPoint.x;
					mouseEvent.location.y = height - windowPoint.y;
					buttonMotionFn(mouseEvent);
				}
					break;
				case NSEventTypeKeyDown:
				case NSEventTypeFlagsChanged:
				default:
					break;
			}

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

		CGDataProviderRef provider = CGDataProviderCreateWithData (NULL, img.data, BitmapMemorySize, 0);
		CGImageRef image = CGImageCreate(width, height, bitsPerComponent, bitsPerPixel,
										bytesPerRow, space, kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little, provider, 0, 0, kCGRenderingIntentDefault);

        nswindow.contentView.layer.contents = (id)image;

		CGImageRelease(image);
		CGDataProviderRelease(provider);
	}
	}

	CGColorSpaceRelease(space);

	[nswindow close];
	[NSApp terminate:nil];
}
}