#include "stdio.h"
#include "tss.h"
#include "ide.h"
#include "string.h"
#include "task.h"
#include "elf.h"
#include "fat12.h"
#include "user.h"
#include "Hal.h"
#include "fat12a.h"


void exec_user_prg(char* name_list)
{
	disable();
	char* name = name_list, *rest = name_list;
	strsep(&rest," ");
  printf("==========1.0=============");
	for (int i = 0; name != NULL; ++i){
		char buf[20];
		strcpy(buf,name);
		char* ext = buf;
		strsep(&ext,".");

		uintptr_t addr_exec;
		memset(bin_img,0,sizeof(bin_img));

		int stack = 0;
		__asm__ (
		".intel_syntax noprefix\n\t"  
		"mov eax, esp\n\t"
		".att_syntax"
		:"=a"(stack)::
		);
		printf("==========1.1=============");
		printf("==========%d=============",stack);
		tss_set_stack(KERNEL_DS,stack+KERNEL_STACK_SIZE);
		if (strcmp(ext,"com") == 0)
		{
		  printf("==========1.2=============");
		  
			if (strcmp(name,"prg1.com") == 0)
			{
			  printf("==========1.2.1=============");
			  //FILE file = volOpenFile (name);
			  
			  printf("before the file prg1.com");
				//volReadFile ( &file, (unsigned char*)addr_exec, 512);
			  printf("readed the file prg1.com");
			  
			  
				addr_exec = 0x20000;
			}
			else if (strcmp(name,"prg2.com") == 0)
				addr_exec = 0x30000;
			else if (strcmp(name,"prg3.com") == 0)
				addr_exec = 0x40000;
			else if (strcmp(name,"prg4.com") == 0)
				addr_exec = 0x50000;
				
				
			if (!fat12_read_file(name,(char*)addr_exec,NULL))
			{
			  printf("==========1.3=============");
				printf("No this user program!");
				return;
			}
			
		} 
		else if (strcmp(ext,"out") == 0 && fat12_read_file(name,(char*)bin_img,NULL)) 
		{
			addr_exec = parse_elf(ADDR_USER_START + 6 * PROC_SIZE);
			printf("==========1.2.3=============");
		} 
		else 
		{
		  printf("==========1.4=============");
		  printf("==========1.2.1=============");
			//FILE file = volOpenFile (name);
			  
			printf("before the file prg1.txt");
			//volReadFile ( &file, (unsigned char*)addr_exec, 512); 
			//vol read is read the file byte by byte
			printf("readed the file prg1.txt");
			  
		  
			//printf("No this user program!");
			//return;
		}

		process* pp = proc_create(USER_CS,USER_DS,addr_exec);
		char info_buf[30];
		sprintf(info_buf,"Created user process %d!",pp->pid);
		put_info(info_buf);

		name = rest;
		strsep(&rest," ");
	}
	// clear_screen();
	schedule_proc();
}

