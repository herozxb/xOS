// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** user.h ******/

#ifndef USER_H
#define USER_H


#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"
#include "tss.h"
#include "ide.h"
#include "string.h"
#include "task.h"
#include "elf.h"
#include "fat12.h"
void exec_user_prg(char* name_list);
#ifdef __cplusplus
}
#endif

#endif // USER_H
