#ifndef _IDT_H
#define _IDT_H
//****************************************************************************
//**
//**    Idt.h
//**		Interrupt Descriptor Table. The IDT is responsible for providing
//**	the interface for managing interrupts, installing, setting, requesting,
//**	generating, and interrupt callback managing.
//**
//****************************************************************************
#pragma once
#ifdef __cplusplus
extern "C"
{
#endif




// We can test new architecture here as needed

#include <stdint.h>
#include "stdio.h"

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================
//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

// i86 defines 256 possible interrupt handlers (0-255)
#define MAX_INTERRUPTS		256
#define INTERRUPT_GATE      0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

// must be in the format 0D110, where D is descriptor type
#define IDT_DESC_BIT16		0x06	//00000110
#define IDT_DESC_BIT32		0x0E	//00001110
#define IDT_DESC_RING1		0x40	//01000000
#define IDT_DESC_RING2		0x20	//00100000
#define IDT_DESC_RING3		0x60	//01100000
#define IDT_DESC_PRESENT	0x80	//10000000

//! interrupt handler w/o error code
//! Note: interrupt handlers are called by the processor. The stack setup may change
//! so we leave it up to the interrupts' implimentation to handle it and properly return
typedef void (*I86_IRQ_HANDLER)(void);

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================

// interrupt descriptor
struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};



//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! returns interrupt descriptor
//extern idt_descriptor* i86_get_ir (uint32_t i);
//! installs interrupt handler. When INT is fired, it will call this callback
//extern int i86_install_ir (uint32_t i, uint16_t flags, uint16_t sel, I86_IRQ_HANDLER);

// initialize basic idt
//extern int i86_idt_initialize (uint16_t codeSel);

//extern void int_handler_5();

//! returns interrupt descriptor
extern IDT_entry* i86_get_ir (uint32_t i);


// default handler to catch unhandled system interrupts.
extern __attribute__((__cdecl__))
void default_handler (unsigned int cs, unsigned int eflags, unsigned int eip);



extern int install_ir (uint32_t i, uint16_t type_attr, uint16_t selector, uint64_t irq);


// sets new interrupt vector
extern void setvect (int intno, uint64_t vect);

extern void setvect_user (int intno, uint64_t vect);

extern void load_idt(unsigned long *idt_ptr);

// initialize basic idt
extern int idt_init();


#ifdef __cplusplus
}
#endif
//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [idt.h]
//**
//****************************************************************************
#endif
