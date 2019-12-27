// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** terminal.h ******/

#ifndef TERMINAL_H
#define TERMINAL_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


#include "stdio.h"
#include "string.h"
#include "DebugDisplay.h"
//#include "user.h"

#define MAX_TERMINAL 4
#define VIDEO_SIZE 80 * 32 * 2



typedef struct terminal
{
	int num;
	int cursor;
	char buf[VIDEO_SIZE];
	char path[50];
	int dir;

} terminal_t;


extern int get_current_terminal();
extern void set_tmp_terminal(int i);
extern void set_at_first_term();

extern uintptr_t get_terminal_buf_addr();
extern void set_cursor(int offset);
extern int get_cursor();

extern void command_not_found(char* str);
extern void put_prompt();

extern void terminal_loop();
extern void new_terminal();
extern void change_terminal(int new_ter);
extern void terminal_init();

#ifdef __cplusplus
}
#endif

#endif // TERMINAL_H
