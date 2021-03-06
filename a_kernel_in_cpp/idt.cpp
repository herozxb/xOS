//****************************************************************************
//**
//**    Idt.cpp
//**		Interrupt Descriptor Table. The IDT is responsible for providing
//**	the interface for managing interrupts, installing, setting, requesting,
//**	generating, and interrupt callback managing.
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "idt.h"
#include "string.h"
#include "Hal.h"
#include "DebugDisplay.h"
#ifdef _DEBUG
#include "DebugDisplay.h"
#endif


struct IDT_entry IDT[MAX_INTERRUPTS];
//IDT_entry 64 bits long, 8 bytes long


unsigned long idt_ptr[2];


//! returns interrupt descriptor
IDT_entry* i86_get_ir (uint32_t i) {

	if (i>MAX_INTERRUPTS)
		return 0;

	return &IDT[i];
}


__attribute__((__cdecl__))
void default_handler (unsigned int cs, unsigned int eflags, unsigned int eip) {
	DebugPrintf (" ===================default_handler======================");
	for (;;);
}


int install_ir (uint32_t i, uint16_t type_attr, uint16_t selector, uint64_t irq) {

	if (i > MAX_INTERRUPTS)
		return 0;

	if (!irq)
		return 0;

	// get base address of interrupt handler
	uint64_t base = irq;

	// store base address into idt
	IDT[i].offset_lowerbits  = (uint16_t)(base & 0xffff);
	IDT[i].offset_higherbits = (uint16_t)((base >> 16) & 0xffff);
	IDT[i].zero              = 0;
	IDT[i].type_attr         = (uint8_t)(type_attr);
	IDT[i].selector          = selector;

	return	0;
}


// sets new interrupt vector
void setvect (int intno, uint64_t vect) {

	// install interrupt handler! This overwrites prev interrupt descriptor
	install_ir (intno, INTERRUPT_GATE, KERNEL_CODE_SEGMENT_OFFSET, vect);
}

void setvect_user (int intno, uint64_t vect) {
	install_ir (intno, INTERRUPT_GATE | IDT_DESC_RING3,
		KERNEL_CODE_SEGMENT_OFFSET, vect);
}


// initialize basic idt
int idt_init() {

	// null out the idt
	memset ((void*)&IDT[0], 0, sizeof(struct IDT_entry) * MAX_INTERRUPTS-1);

	// register default handlers
	for (int i = 0; i < MAX_INTERRUPTS; i++)
		setvect (i, (unsigned long)default_handler);

	// fill the IDT register
	unsigned long idt_address = (unsigned long)IDT; // unsigned long is 64 bits or 8 bytes long
	idt_ptr[0] = (sizeof(struct IDT_entry) * MAX_INTERRUPTS) + ((idt_address & 0xffff) << 16); //0xffff 16 bits long
	idt_ptr[1] = idt_address >> 16;

	load_idt(idt_ptr); // assembly
	
//	load_idt:
//	mov eax, [ esp + 4 ] ;esp is 32 bits long, 4 * 32 = 2 * 64 = two long
//	lidt [ eax ]        ; load interrupt description table (IDT)
//	sti                 ; turn on interrupts
//	ret


// 1bf:	e8 fc ff ff ff       	call   1c0 <idt_init+0x7e>
// 1c4:	83 c4 10             	add    $0x10,%esp  ;0x10 = 16 bits esp = 32 bits
// 1c7:	b8 00 00 00 00       	mov    $0x0,%eax


	return 0;
}


//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[idt.cpp]
//**
//****************************************************************************
