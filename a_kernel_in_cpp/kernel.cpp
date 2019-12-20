/* This will force us to create a kernel entry function instead of jumping to kernel.c:0x00 */

#include "Hal.h"
#include "DebugDisplay.h"
#include "idt.h"


extern "C" void main() {
  
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
	geninterrupt (0x15);

//! registers our interrupt handler
//i86_install_ir (5, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,0x8, (I86_IRQ_HANDLER)int_handler_5);
 
//! generates int 5 instruction. You can also use inline assembly, of course
//geninterrupt (5);
}
