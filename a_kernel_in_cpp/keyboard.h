// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** keyboard.h ******/
// https://github.com/arjun024/mkeykernel/blob/master/kernel.c

#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "io.h"
#include "scancode.h"
#include "DebugDisplay.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define INVALID_KB_CHAR 0

//static char kb_char;
//static bool __ctrl = false;


extern void keyboard_handler_main(void);
extern char getchar();

#ifdef __cplusplus
}
#endif

#endif // KEYBOARD_H
