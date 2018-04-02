#include "./include/types.h"
#include "./include/print.h"
#include "./include/port.h"
#include "./include/globalDescriptorTable.h"
#include "./include/interrupts.h"
#include "test.h"

extern "C" void kernelMain(const void* multiboot_structure, uint32_t multiboot_magic)
{
    	printf("Hello World! Write my first OS by hand from ZXB\n");
	////////////////////////////////////////////////////////////////////////////////////
	//setup the Table
    	GlobalDescriptorTable gdt;
	setupSegmentDescriptor((uint8_t*) &(gdt.nullSegmentSelector),0,0,0);
	setupSegmentDescriptor((uint8_t*) &(gdt.unusedSegmentSelector),0,0,0);
	setupSegmentDescriptor((uint8_t*) &(gdt.codeSegmentSelector),0,64*1024*1024,0x9A);
	setupSegmentDescriptor((uint8_t*) &(gdt.dataSegmentSelector),0,64*1024*1024,0x92);
	loadTable(&gdt);
	dataSegmentSelector(&gdt);
	codeSegmentSelector(&gdt);
	Base(&gdt);
	Limit(&gdt);
	printf("at the end of GDT\n");
	//
	////////////////////////////////////////////////////////////////////////////////////
	printf("before the interrupt\n");
	uint16_t hardwareInterruptOffset_in = 0x20;
	static GateDescriptor interruptDescriptorTable[256];
	interruptManager(hardwareInterruptOffset_in, &gdt,interruptDescriptorTable);
	Activate();
	printf("at the end of program\n");

    	while(1);
}

