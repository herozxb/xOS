
//****************************************************************************
//**
//**    gdt.cpp
//**
//**	global descriptor table (gdt) for i86 processors. This handles
//**	the basic memory map for the system and permission levels
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "gdt.h"
//#include "string.h"
//#include "tss.h"

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================


//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

// global descriptor table is an array of descriptors
static struct gdt_descriptor	_gdt[MAX_DESCRIPTORS];

// gdtr data
struct gdtr				_gdtr; // used for assembly
struct tss_entry tss={0};

//! install gdtr
static void gdt_install ();

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

#include "string.h"
//! Setup a descriptor in the Global Descriptor Table
void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand)
{
	if (i > MAX_DESCRIPTORS)
		return;

	//! null out the descriptor
	memset ((void*)&_gdt[i], '0', sizeof (gdt_descriptor));

	//! set limit and base addresses
	_gdt[i].baseLo	= uint16_t(base & 0xffff);
	_gdt[i].baseMid	= uint8_t((base >> 16) & 0xff);
	_gdt[i].baseHi	= uint8_t((base >> 24) & 0xff);
	_gdt[i].limit	= uint16_t(limit & 0xffff);

	//! set flags and grandularity bytes
	_gdt[i].flags = access;
	_gdt[i].grand = uint8_t((limit >> 16) & 0x0f);
	_gdt[i].grand |= grand & 0xf0;
}


//! returns descriptor in gdt
gdt_descriptor* gdt_get_descriptor (int i) {

	if (i > MAX_DESCRIPTORS)
		return 0;

	return &_gdt[i];
}

void tss_init(uint32_t sel) {
	uint32_t base = (uint32_t) &tss;
	gdt_set_descriptor (sel, base, base + sizeof(tss),
		GDT_DESC_ACCESS|GDT_DESC_EXEC_CODE|GDT_DESC_DPL|GDT_DESC_MEMORY,
		0);
}


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


//! initialize gdt
int gdt_init () {

	//! set up gdtr
	_gdtr.m_limit = (sizeof (struct gdt_descriptor) * MAX_DESCRIPTORS)-1;
	_gdtr.m_base = (uint32_t)&_gdt[0];

	//! set null descriptor
	gdt_set_descriptor(0, 0, 0, 0, 0);


	// set default code descriptor (0x08)
	gdt_set_descriptor (1,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_EXEC_CODE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	// set default data descriptor (0x10)
	gdt_set_descriptor (2,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	// set default user mode code descriptor (0x18)
	gdt_set_descriptor (3,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_EXEC_CODE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY|GDT_DESC_DPL,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	// set default user mode data descriptor (0x20)
	gdt_set_descriptor (4,0,0xffffffff,
		GDT_DESC_READWRITE|GDT_DESC_CODEDATA|GDT_DESC_MEMORY|GDT_DESC_DPL,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK);

	tss_init(5);
	load_gdt(); // assembly
	tss_install(5);
	
	return 0;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[gdt.cpp]
//**
//****************************************************************************
