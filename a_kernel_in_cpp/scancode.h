// Source from http://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/scancode
// and https://github.com/arjun024/mkeykernel/blob/master/keyboard_map.h
// https://github.com/jaytaph/CybOS/blob/master/source/kernel/keyboard.c

#ifndef SCANCODE_H
#define SCANCODE_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#define ESC 27     /* ASCII escape */

/* Scancodes for shift and capslock keys: */

#define LSHIFT_PRESSED 0x2A          /* Scan codes for shift press, */
#define LSHIFT_RELEASED 0xAA        /* shift release and capslock  */
#define RSHIFT_PRESSED 0x36        /* keys.                       */
#define RSHIFT_RELEASED 0xB6
#define CAPSLOCK 0x3A
#define CAPSLOCK_RELEASED 0xBA 

/* scancodes for arrow keys: */

#define CODE_UP    72
#define CODE_DOWN  80
#define CODE_LEFT  75
#define CODE_RIGHT 77

/* scancodes for function keys (for project 5) */

#define KEY_CTRL 0x1D

#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define KEY_F3 0x3D

/* ASCII codes to use for arrow keys: */

#define ASC_UP    '\200'
#define ASC_DOWN  '\201'
#define ASC_LEFT  '\202'
#define ASC_RIGHT '\203'

#define BACKSPACE  8       /* Ascii codes for Backspace, Tab and enter */
#define TAB        9      /* keys.                                    */
#define ENTER_KEY  13

/*  Note, not every single ASCII character has a scancode.  Only,
    the first 58 do. */

#ifdef __cplusplus
}
#endif
#endif // SCANCODE_H
