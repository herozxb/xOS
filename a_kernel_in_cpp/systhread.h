// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** systhread.h ******/

#ifndef SYSTHREAD_H
#define SYSTHREAD_H



#pragma once
#ifdef __cplusplus
extern "C"
{
#endif




#include "task.h"
#include <stdint.h>

extern void user_pthread_return();

extern int do_thread_create(int* tid, uintptr_t func, void* args);

extern void do_thread_join(int tid, void** ret);

extern int do_thread_self();

extern void do_thread_exit();

extern void user_pthread_return();

extern void sys_pthread_handler ();
extern int sys_pthread_handler_main (int no);

#ifdef __cplusplus
}
#endif

#endif // SYSTHREAD_H
