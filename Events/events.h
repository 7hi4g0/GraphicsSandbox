#ifndef	EVENTS_H
#define EVENTS_H

#include <Events/keycodes.h>
#include <Point/point.h>

typedef struct KeyboardEvent {
    KeyCode keycode;
} KeyboardEvent;

typedef struct MouseEvent {
    Point location;
} MouseEvent;

#endif