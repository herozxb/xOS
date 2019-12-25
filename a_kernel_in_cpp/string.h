// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** string.h ******/

#ifndef _STRING_H_1
#define _STRING_H_1

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif



#include "size_t.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern char *strcpy(char *s1, const char *s2);
extern size_t strlen ( const char* str );

// sets count bytes of dest to val
extern void *memset(void *dest, char val, size_t count);

extern void *memcpy(void *dst, const void *src, size_t n);

extern size_t strlen(const char* str);

extern int strcmp(const char *s1, const char *s2);

extern char* strcpy(char *strDest, const char *strSrc);

extern char* strncpy(char *dest, const char *src, size_t n);

extern char* strmcpy(char *dest, const char *src, size_t start, size_t end);
extern char* strcat(char* dst, const char* src);

extern int atoi(const char *str) ;
extern void reverse(char* s);

// Find the first occurrence of a character in a string.
extern char *strchr(const char *s, char c);

extern char *strsep(char **str, const char *delims);

extern void itoa(unsigned i, char* buf, unsigned base) ;

extern void itoa_s(int i, char* buf, unsigned base);
extern char* tolower(char* str);
extern char* toupper(char* str);

extern bool isalpha(char c);
extern bool isdigit(char c);

extern bool isspace(char c);
extern bool isnum(char* str);

extern bool isin(char* str, char c);

extern int split(const char* str, char c, int* offset);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
