// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** ide.h ******/

#ifndef IDE_H
#define IDE_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


#define SECTSIZE 512
#define TMP_USER_ADDR 0x1000



#include "string.h"

extern void insw(uint32_t port, uintptr_t addr, int cnt) ;
extern void outsw(uint32_t port, uintptr_t addr, int cnt) ;

/* waitdisk - wait for disk ready */
extern void waitdisk(void);

/* readsect - read a single sector at @secno into @dst */
extern void readsect(uintptr_t dst, uint32_t secno) ;

// read [@startsec,@startsec+@cnt] sectors to @addr
extern void read_sectors(uintptr_t addr, uint32_t startsec, uint32_t cnt);
extern void writesect(uintptr_t src, uint32_t secno) ;

// write @addr to [@startsec,@startsec+@cnt]
extern void write_sectors(uintptr_t addr, uint32_t startsec, uint32_t cnt);

extern void show_one_sector(uintptr_t addr);

extern void show_hex(uintptr_t addr, size_t cnt);

extern void read_disk_test();

#ifdef __cplusplus
}
#endif

#endif // IDE_H
