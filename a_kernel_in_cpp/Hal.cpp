
//****************************************************************************
//**
//**    Hal.cpp
//**		Hardware Abstraction Layer for i86 architecture
//**
//**	The Hardware Abstraction Layer (HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "Hal.h"
#include "cpu.h"
#include "idt.h"

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

//! initialize hardware devices
int hal_initialize () {

	i86_cpu_initialize ();
	return 0;
}

//! shutdown hardware devices
int hal_shutdown () {

	i86_cpu_shutdown ();
	return 0;
}

//! generate interrupt call
void geninterrupt (int n) {
#ifdef _MSC_VER
	_asm {
		mov al, byte ptr [n]
		mov byte ptr [genint+1], al
		jmp genint
	genint:
		int 0	// above code modifies the 0 to int number to generate
	}
#endif
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[Hal.cpp]
//**
//****************************************************************************
