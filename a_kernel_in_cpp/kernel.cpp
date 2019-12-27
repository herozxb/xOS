/* This will force us to create a kernel entry function instead of jumping to kernel.c:0x00 */

#include "Hal.h"
#include "DebugDisplay.h"
#include "idt.h"
#include "bootinfo.h"
#include "mmngr_phys.h"
#include "mmngr_virtual.h"
//#include "kybrd.h"
#include "string.h"
#include "terminal.h"

/*
*	Our uber-1337 logo
*/
char* logo =
"\
    __  _______  _____\n\
   /  |/  / __ \\/ ___/\n\
  / /|_/ / / / /\\__ \\ Microcomputer Operating System \n\
 / /  / / /_/ /___/ / -------------------------------\n\
/_/  /_/\\____//____/  \n\n";


//! format of a memory region
struct memory_region {

	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	type;
	uint32_t	acpi_3_0;
};

//! different memory regions (in memory_region.type)


char strMemoryTypes[] = {

	'A',//"Available\n",			//memory_region.type==0
	'R',//"Reserved\n",			//memory_region.type==1
	'R',//"ACPI Reclaim\n",		//memory_region.type==2
	'M'//"ACPI NVS Memory\n"		//memory_region.type==3
};

extern "C" void main(multiboot_info* bootinfo) {
  
	hal_initialize ();
	
	clear_screen();
	terminal_init();
	terminal_loop();
}
