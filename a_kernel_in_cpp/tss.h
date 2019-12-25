// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** tss.h ******/

#ifndef tss_H
#define tss_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


#include "stdio.h"
#include "string.h"
#include <stdint.h>

#define KERNEL_STACK_SIZE 2048		 // Use a 2kb kernel stack

/* global segment number */
#define SEG_KTEXT   1
#define SEG_KDATA   2
#define SEG_UTEXT   3
#define SEG_UDATA   4
#define SEG_TSS     5

/* global descrptor numbers */
#define GD_KTEXT    ((SEG_KTEXT) << 3)      // kernel text
#define GD_KDATA    ((SEG_KDATA) << 3)      // kernel data
#define GD_UTEXT    ((SEG_UTEXT) << 3)      // user text
#define GD_UDATA    ((SEG_UDATA) << 3)      // user data
#define GD_TSS      ((SEG_TSS) << 3)        // task segment selector

/*
 * CPL: Current Privilege Level
 * RPL: Request Privilege Level
 * DPL: Descriptor Privilege Level
 */
#define DPL_KERNEL  (0)
#define DPL_USER    (3)

#define KERNEL_CS   ((GD_KTEXT) | DPL_KERNEL) // 0x08
#define KERNEL_DS   ((GD_KDATA) | DPL_KERNEL) // 0x10
#define USER_CS     ((GD_UTEXT) | DPL_USER)   // 0x18 | 3
#define USER_DS     ((GD_UDATA) | DPL_USER)   // 0x20 | 3

/*
 * When operating in protected mode, a TSS and TSS descriptor 
 * MUST be created for at least one task, and the
 * segment selector for the TSS must be loaded into
 * the task register (using the LTR instruction).
 */
struct tss_entry {
	uint32_t prevTss;
	uint32_t esp0;
	uint32_t ss0; // ring 0
	uint32_t esp1;
	uint32_t ss1; // ring 1
	uint32_t esp2;
	uint32_t ss2; // ring 2
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap;
} __attribute__((packed));

//extern struct tss_entry tss;

extern void tss_install (uint32_t sel);

extern void tss_set_stack (uint32_t ss, uint32_t esp);

#ifdef __cplusplus
}
#endif

#endif // tss_H
