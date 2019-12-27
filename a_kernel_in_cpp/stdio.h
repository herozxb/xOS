// Copyright(c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

#ifndef STDIO_H
#define STDIO_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


#include "io.h"
#include "string.h"
#include "keyboard.h"
#include <stdarg.h>

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

#define MAX_BUF_LEN 1000

#define DEFAULT_COLOR 0x0007
enum Color{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10, //a
	LIGHT_CYAN = 11, // b
	LIGHT_RED = 12, // c
	LIGHT_MAGENTA = 13, // d
	LIGHT_BROWN = 14, // e
	WHITE = 15 // f
};


/* Copy bytes from one place to another */
extern void memory_copy(char* source, char* dest, int no_bytes) ;
extern int get_color(enum Color fg, enum Color bg);

extern void set_color(enum Color fg, enum Color bg);

extern int get_screen_offset(int col,int row);
// implemented in terminal.h
extern void set_cursor(int offset);
extern int get_cursor();

extern uintptr_t get_terminal_buf_addr();
/* Advance the text cursor, scrolling the video buffer if necessary */
extern int handle_scrolling(int cursor_offset) ;
/* Print a char on the screen at col, row, or at cursor position */
extern void putchar(char character, int col, int row, char attribute_byte);

extern void print_at(const char* message, int col, int row);

extern void print(const char* message) ;

extern void print_char(const char c) ;
extern void print_int(const int x, const int base);

extern void clear_screen() ;
extern char getchar(); // defined in keyboard.h

extern void getline(char* res);

extern int read_int(const char* s, int* readNum) ;

extern void sprintf(char* buf, const char* format, ...) ;
extern void printf(const char* format, ...);

// sscanf("info:abc num:123","info:%s num:%d",str,&num)
extern void sscanf(const char* s, const char* format, ...);
extern void scanf(const char* format, ...) ;

extern void put_error(char* str);

extern void put_exception(char* str);
extern void put_info(char* str);
extern void show_static_string(char* str,int row); // assembly

#ifdef __cplusplus
}
#endif

#endif // STDIO_H
