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

#include "fat12a.h"
#include "fat12.h"
//#include "sysfile.h"

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
	//printf("=============1.0==============\n");
	flpydsk_install (38);

	//! initialize FAT12 filesystem
	fsysFatInitialize ();
	fat12_init();
	//printf("=============1.1==============\n");	
	
/*
	uint8_t* sector = 0;
	int sectornum=0;
	printf ("\n\rSector %i contents:\n\n\r", sectornum);

    while(1)
	{
		//! read sector from disk
		sector = flpydsk_read_sector ( sectornum );

		//! display sector
		if (sector!=0) {

			int i = 0;
			for ( int c = 0; c < 1; c++ ) {

				for (int j = 0; j < 128; j++)
				{
					printf ("0x%x ", sector[ i + j ]);

					if(sector[ i + j ] == 'H'&&sector[ i + j + 1 ] == 'e')
					{
						printf("==================================\n\r");
						printf("sectornum = %d",sectornum);
						printf("name is =[%c%c%c%c%c%c%c%c]",sector[ i + j ],sector[ i + j +1  ],sector[ i + j +2  ],sector[ i + j +3 ],sector[ i + j + 4 ],sector[ i + j +5  ],sector[ i + j + 6  ],sector[ i + j + 7 ]);
						goto end;

					}
				}
				i += 128;

				//printf("\n\rPress any key to continue\n\r");
			}
		}
		else
			printf ("\n\r*** Error reading sector from disk");
		sectornum = sectornum+1;
		printf("===sectornum = %d===\n",sectornum);
	}


		//! read sector from disk
		
	sector = flpydsk_read_sector ( 117 );

	//! display sector
	if (sector!=0) {

		int i = 0;
		for ( int c = 0; c < 0; c++ ) {

			for (int j = 0; j < 128; j++)
			{
				printf ("%c ", sector[ i + j ]);

				if(sector[ i + j ] == 'H'&&sector[ i + j + 1 ] == 'e')
				{
					printf("==================================\n\r");
					printf("sectornum = %d",sectornum);
					printf("name is =[%c%c%c%c%c%c%c%c]",sector[ i + j ],sector[ i + j +1  ],sector[ i + j +2  ],sector[ i + j +3 ],sector[ i + j + 4 ],sector[ i + j +5  ],sector[ i + j + 6  ],sector[ i + j + 7 ]);
					//goto end;

				}

			}
			i += 128;

			//printf("\n\rPress any key to continue\n\r");
		}
	}
	else
		printf ("\n\r*** Error reading sector from disk");

	//sectornum = sectornum+1;
	printf("===sectornum = %d===\n",sectornum);
	//goto end;
	//*/
	//sectornum = 19  FILE   TXT
	//sectornum = 117 Hello World!

			//! get pathname
	//char path[32];
	//printf ("\n\rex: \"file.txt\", \"a:\\file.txt\", \"a:\\folder\\file.txt\"\n\rFilename> ");
		//*/
	//clear_screen();
	terminal_init();
	terminal_loop();


	//end:
	//	int i = 0;
}
