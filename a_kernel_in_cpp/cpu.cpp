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
#include "flpydsk.h"
#include "syscall.h"
#include "systhread.h"

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


//! our uber 1337 interrupt handler. handles int 5 request
//void int_handler_5 () {
//    DebugPrintf ("interrupt is ok! for 5");
//}
 
//void geninterrupt(int n){
//	__asm__ (
//	    ".intel_syntax noprefix \n\t"
//			"mov byte ptr [genint+1], al\n\t"
//			"jmp genint\n"
//			"genint:\n\t"
//			"int 0\n\t" // above code modifies the 0 to int number to generate
//			".att_syntax"
//			:
//			:"a"(n)
//			);
//}




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
	setvect (38, (uint64_t)i86_flpy_irq);


	//setvect_user (0x80, (uint64_t)syscall_dispatcher);
	
	// install my own interrupts
	setvect_user (0x80, (unsigned long) sys_interrupt_handler);
	setvect_user (0x81, (unsigned long) sys_pthread_handler);
	setvect_user (0x82, (unsigned long) sys_file_handler);

//! generates int 5 instruction. You can also use inline assembly, of course
  //geninterrupt (5);
	DebugPrintf("Initialized IDT\n");




}

//! shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown () {


}


//! returns vender name of cpu
char* i86_cpu_get_vender () {

	static char	vender[32] = {0};

#ifdef _MSC_VER
	_asm {
		mov		eax, 0
		cpuid
		mov		dword ptr [vender], ebx
		mov		dword ptr [vender+4], edx
		mov		dword ptr [vender+8], ecx
	}
#endif

	return vender;
}


//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[String.cpp]
//**
//****************************************************************************
