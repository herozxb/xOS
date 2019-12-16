/* This will force us to create a kernel entry function instead of jumping to kernel.c:0x00 */


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
  
	int i=0x12;

	DebugClrScr (0x18);

	DebugGotoXY (4,4);
	DebugSetColor (0x17);
	DebugPrintf ("+-----------------------------------------+\n");
	DebugPrintf ("|    MOS 32 Bit C++ Kernel Executing!     |\n");
	DebugPrintf ("+-----------------------------------------+\n\n");
	DebugSetColor (0x12);

	DebugSetColor (0x12);
	DebugPrintf ("\ni as integer ........................");
	DebugPrintf ("\ni in hex ............................");

	DebugGotoXY (25,8);
	DebugSetColor (0x1F);
	DebugPrintf ("\n[%i]",i);
	DebugPrintf ("\n[0x%x]",i);

	DebugGotoXY (4,16);
	DebugSetColor (0x1F);
	DebugPrintf ("\n\nI am preparing to load... Hold on, please... :)");
}
