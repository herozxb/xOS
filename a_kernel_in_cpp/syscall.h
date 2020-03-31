// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** syscall.h ******/

#ifndef SYSCALL_H
#define SYSCALL_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

//#include "task.h"
//#include "semaphore.h"
//#include "sysfile.h"
#include "Hal.h"



extern int sys_interrupt_handler_main (int no);
extern void sys_interrupt_handler ();



extern int sys_file_handler_main (int no);
extern void sys_file_handler ();

#ifdef __cplusplus
}
#endif

#endif // SYSCALL_H
