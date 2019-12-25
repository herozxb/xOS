//****************************************************************************
//**
//**    cpu.cpp
//**
//**	This is the processor interface. Everything outside of this module
//**	must use this interface when working on processor data.
//**
//**	A processor is a module that manages the very basic data structures
//**	and data within the system. The processor interface provides the interface
//**	for managing processors, processor cores, accessing processor structures,
//**	and more
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "cpu.h"
#include "gdt.h"
#include "idt.h"
#include "DebugDisplay.h"
#include "exception.h"

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
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! Initializes cpu resources
int i86_cpu_initialize () {
	/*
	 * Terminal initialzation
	 */
	//set_at_first_term();

	DebugPrintf("\n");
	/*
	 * Global Descriptor Table (GDT) and task state segment (TSS) initialization
	 */
	gdt_init(); // re-implement for completeness
	DebugPrintf("Initialized GDT & TSS\n");

	/*
	 * Interrupt Descriptor Table (IDT) initialization
	 */
	idt_init();

	// install exception handlers
	setvect (0, (unsigned long) divide_by_zero_fault);
	setvect (1, (unsigned long) single_step_trap);
	setvect (2, (unsigned long) nmi_trap);
	setvect (3, (unsigned long) breakpoint_trap);
	setvect (4, (unsigned long) overflow_trap);
	setvect (5, (unsigned long) bounds_check_fault);
	setvect (6, (unsigned long) invalid_opcode_fault);
	setvect (7, (unsigned long) no_device_fault);
	setvect (8, (unsigned long) double_fault_abort);
	setvect (10, (unsigned long) invalid_tss_fault);
	setvect (11, (unsigned long) no_segment_fault);
	setvect (12, (unsigned long) stack_fault);
	setvect (13, (unsigned long) general_protection_fault);
	setvect (14, (unsigned long) page_fault);
	setvect (16, (unsigned long) fpu_fault);
	setvect (17, (unsigned long) alignment_check_fault);
	setvect (18, (unsigned long) machine_check_abort);
	setvect (19, (unsigned long) simd_fpu_fault);

	// install my own interrupts
	//setvect_user (0x80, (unsigned long) sys_interrupt_handler);
	//setvect_user (0x81, (unsigned long) sys_pthread_handler);
	//setvect_user (0x82, (unsigned long) sys_file_handler);

	DebugPrintf("Initialized IDT\n");




}

//! shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown () {


}


//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[String.cpp]
//**
//****************************************************************************
