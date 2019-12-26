/******************************************************************************

	entry.cpp
		-Kernel entry point called from boot loader

******************************************************************************/

#include "bootinfo.h"

//! basic crt initilization stuff
extern void  InitializeConstructors ();
extern void  Exit ();

//! main
extern int kmain (multiboot_info* bootinfo);

//! kernel entry point is called by boot loader
extern void  kernel_entry (multiboot_info* bootinfo) {


	__asm__ volatile (
	    ".intel_syntax noprefix \n\t"
			"cli\n\t"
			"mov ax, 10h\n\t"
			"mov ds, ax\n\t"
			"mov es, ax\n\t"
			"mov fs, ax\n\t"
			"mov gs, ax\n\t"
			".att_syntax"
			);


	kmain (bootinfo);

	__asm__ volatile (
	    ".intel_syntax noprefix \n\t"
			"cli\n\t"
			"hlt\n\t"
			".att_syntax"
			);


	for (;;);
}
