#ifndef	KEYCODES_H
#define KEYCODES_H

typedef enum {
#ifdef __APPLE__
    KEYCODE_A = 0,
    KEYCODE_S = 1,
    KEYCODE_C = 8,
    KEYCODE_R = 15,
    KEYCODE_EQUALS = 24,
    KEYCODE_MINUS = 27,
    KEYCODE_P = 35
#else // TODO: If Linux
    KEYCODE_A = XK_a,
    KEYCODE_S = XK_s,
    KEYCODE_C = XK_c,
    KEYCODE_R = XK_r,
    KEYCODE_EQUALS = XK_KP_Add, // FIX: This is wrong
    KEYCODE_MINUS = XK_KP_Subtract,
    KEYCODE_P = XK_p
#endif
} KeyCode;

#endif