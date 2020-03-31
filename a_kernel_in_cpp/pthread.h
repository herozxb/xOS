// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** pthread.h ******/

#ifndef PTHREAD_H
#define PTHREAD_H

#include <stdint.h>
typedef int pthread_t;
typedef uintptr_t pthread_addr;

int pthread_create(int* tid, uintptr_t func, void* args) {
	int ret;
	__asm__(
    ".intel_syntax noprefix \n\t"  
		"int 0x81\n\t"
		".att_syntax"
		:"=a"(ret)
		:"a"(0),"b"(tid),"c"(func),"d"(args)
		);
	return ret;
}

void pthread_join(int tid, void** ret) {
	__asm__(
	  ".intel_syntax noprefix \n\t"  
		"int 0x81\n\t"
		".att_syntax"
		:
		:"a"(1),"b"(tid),"c"(ret)
		);
}

int pthread_self() {
	int tid;
	__asm__(
	  ".intel_syntax noprefix \n\t"  
		"int 0x81\n\t"
		".att_syntax"
		:"=a"(tid)
		:"a"(2)
		);
	return tid;
}

void pthread_exit() {
	__asm__(
	  ".intel_syntax noprefix \n\t"  
		"int 0x81\n\t"
		".att_syntax"
		:
		:"a"(3)
		);
}

#endif // PTHREAD_H
