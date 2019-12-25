#include "tss.h"

/*
struct tss_entry tss={0};

void tss_install (uint32_t sel) {

	memset((void *)&tss, 0, sizeof(tss));

	// Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what
	// segments should be loaded when the processor switches to kernel mode. Therefore
	// they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
	// but with the last two bits set, making 0x0b and 0x13. The setting of these bits
	// sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
	// to switch to kernel mode from ring 3.
	tss.ss0 = KERNEL_DS;
	tss.esp0 = 0x0;
	tss.cs = KERNEL_CS | DPL_USER;
	tss.ss = tss.es = tss.ds = tss.fs = tss.gs = KERNEL_DS | DPL_USER;

	__asm__ volatile (
		  ".intel_syntax noprefix \n\t"
			"ltr ax\n\t"
			".att_syntax"
			:
			:"a"(sel << 3)
			:
			);
}

void tss_set_stack (uint32_t ss, uint32_t esp) {

	// memset((void *)&tss, 0, sizeof(tss));
	tss.ss0 = ss;
	tss.esp0 = esp;
	// tss.ebp = ebp;
	// tss.iomap = sizeof(tss);
}
*/
