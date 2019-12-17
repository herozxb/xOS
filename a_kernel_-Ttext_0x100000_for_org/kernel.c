/* This will force us to create a kernel entry function instead of jumping to kernel.c:0x00 */

#include "Hal.h"
#include "DebugDisplay.h"


void dummy_test_entrypoint() {
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'A';
}


void dummy_test_entrypoint2() {
	for (int i = 0; i < 26; i++) {
		char c = 0x41 + i;

		asm(
			"mov %0, %%al;"
			"mov $0x0E, %%ah;"
			"int $0x10;"
			:
			: "r" (c)
		);
	}
}
//*/

void main() {
  
	DebugClrScr (0x18);

	DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" Microcomputer Operating System (MOS) Preparing to load...                       ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" MOS Starting Up...\n");

	DebugSetColor (0x70);
	DebugGotoXY (0,24);
	DebugPrintf (" Initializing Hardware Abstraction Layer (HAL.lib)...                           ");

	DebugSetColor (0x19);
	DebugGotoXY (0,2);

	hal_initialize ();

//! uncomment to generate interrupt 0x15. This will call the default exception handler
//	geninterrupt (0x15);
}
