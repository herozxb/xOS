// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** task.h ******/

#ifndef TASK_H
#define TASK_H



#pragma once
#ifdef __cplusplus
extern "C"
{
#endif




#include "stdio.h"
#include "gdt.h"
#include "tss.h"
#include <stdint.h>

/*
0x00000000-0x00400000 – Kernel reserved
0x00400000-0x80000000 – User land
0x80000000-0xffffffff – Kernel reserved
*/
// #define ADDR_USER_START   0x00400000
#define ADDR_USER_START   0x0020000
// #define ADDR_KERNEL_START 0x80000000
#define PROC_SIZE 0x10000 // 64k

#define PRIOR_SYS  0x2
#define PRIOR_USER 0x1

#define MAX_PROCESS 20
#define MAX_TICK 1

/*
 * Five-state process model
 *
 *                <----------timer------------
 *               /                            \
 * NEW -fork-> READY -------restart-------> RUNNING -exit-> TERMINATED
 *               \                            /
 *                <-wakeup- WAITING <-blocked-
 */

enum PROC_STATUS
{
	PROC_NEW = 0,
	PROC_READY = 1,
	PROC_RUNNING = 2,
	PROC_WAITING = 3, // blocked
	PROC_TERMINATED = 4 // exit
};

struct regs {
	// segment register
	uint32_t gs; // 16-bit
	uint32_t fs; // 16-bit
	uint32_t es; // 16-bit
	uint32_t ds; // 16-bit

	// general registers, saved by `pusha`
	// the order should be the same
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp; // stack
	uint32_t edx;
	uint32_t ecx;
	uint32_t ebx;
	uint32_t eax;

	// saved by `int` (interrupt)
	uint32_t eip;
	uint32_t cs; // 16-bit
	uint32_t eflags;
	uint32_t user_esp;
	uint32_t ss; // 16-bit
} __attribute__((packed));
typedef struct regs regs;

/*
 * Reference to Linux
 * https://stackoverflow.com/questions/807506/threads-vs-processes-in-linux
 * View threads as light-weight processes
 */
typedef struct process {
	regs   regImg;
	int    pid;
	struct process* parent;
	struct process* next; // used for semaphore
	int    priority;
	int    status;
	int    tick;
	int    terminal;
} process;





extern void print_proc_info(process* pp);
extern process* get_proc();
extern void reset_time(process* pp);

extern int sys_get_pid();

extern void proc_init();

// NEW -> READY
extern process* proc_alloc();
extern int get_current_terminal();
extern void user_process_return();
extern process* proc_create(uint32_t cs, uint32_t ds, uintptr_t addr);
extern process* proc_pick();
extern void restart_proc(
	uint32_t gs, uint32_t fs, uint32_t es, uint32_t ds,
	uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t esp,
	uint32_t ebx, uint32_t edx, uint32_t ecx, uint32_t eax,
	uint32_t eip,
	uint32_t cs,
	uint32_t eflags,
	uint32_t user_esp,
	uint32_t ss
	);

extern void enter_usermode(uintptr_t addr);

extern void save_proc(
	uint32_t gs,uint32_t fs,uint32_t es,uint32_t ds,
	uint32_t di,uint32_t si,uint32_t bp,uint32_t sp,
	uint32_t bx,uint32_t dx,uint32_t cx,uint32_t ax,
	uint32_t ip,uint32_t cs,uintptr_t flags,
	uint32_t user_esp, uint32_t ss);

extern void set_tmp_terminal(int i);
// READY -> RUNNING && RUNNING -> READY
extern void proc_switch(process* pp);

extern void schedule_proc();

// pit timer interrupt handler
extern void pit_handler_main(
	uint32_t gs,uint32_t fs,uint32_t es,uint32_t ds,
	uint32_t di,uint32_t si,uint32_t bp,uint32_t sp,
	uint32_t bx,uint32_t dx,uint32_t cx,uint32_t ax,
	uint32_t ip,uint32_t cs,uintptr_t flags,
	uint32_t user_esp, uint32_t ss);
extern uint32_t read_eip();

// NEW -> READY
extern int do_fork();
// initiative, WAITING -> READY
extern void wakeup(uint8_t pid);
// passive blocked
// void blocked()

// wait for child process
extern void do_wait();
// RUNNING -> TERMINATED
extern void do_exit();
extern int kill(uint8_t pid);
extern void user_process_return();
#ifdef __cplusplus
}
#endif


#endif // TASK_H
