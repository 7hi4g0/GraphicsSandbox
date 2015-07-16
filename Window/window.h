#ifndef	WINDOW_H
#define WINDOW_H

#include <Image/image.h>

#define KEY_RELEASE(name)		void name(XKeyEvent xkey)
#define BUTTON_RELEASE(name)	void name(XButtonEvent xkey)

typedef KEY_RELEASE(KeyReleaseFn);
typedef BUTTON_RELEASE(ButtonReleaseFn);

extern char windowName[];
extern char fileName[];
extern uint32_t width, height;
extern KeyReleaseFn *keyReleaseFn;
extern ButtonReleaseFn *buttonReleaseFn;

extern void prepare(void);
extern void draw(Image image);

#endif
