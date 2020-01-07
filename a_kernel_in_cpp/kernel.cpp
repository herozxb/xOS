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
#include "flpydsk.h"

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
  clear_screen();
	hal_initialize ();
	flpydsk_set_working_drive (0);
	printf("=============1.0==============\n");
	flpydsk_install (38);
	
	uint8_t* sector = 0;
	int sectornum=0;
	printf ("\n\rSector %i contents:\n\n\r", sectornum);

	//! read sector from disk
	sector = flpydsk_read_sector ( sectornum );

	//! display sector
	if (sector!=0) {

		int i = 0;
		for ( int c = 0; c < 4; c++ ) {

			for (int j = 0; j < 128; j++)
				printf ("0x%x ", sector[ i + j ]);
			i += 128;

			printf("\n\rPress any key to continue\n\r");
		}
	}
	else
		printf ("\n\r*** Error reading sector from disk");
	
	//clear_screen();
	terminal_init();
	terminal_loop();
}
