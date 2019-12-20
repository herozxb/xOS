
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
#include "DebugDisplay.h"

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
    DebugPrintf ("\n[0x%x]",n);

/*
    asm volatile (
        ".intel_syntax noprefix \n\t"

        "mov byte ptr [genint+1], 5\n\t" 
        "genint: int 0\n\t"
        ".att_syntax"

        );
//*/

    asm volatile (
        ".intel_syntax noprefix \n\t"
        "mov byte ptr [genint+1], al\n\t" 
        "genint: int 0\n\t"
        ".att_syntax"
        ::"a"(n)    
        );
  //*/      
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

/*
00000024 <geninterrupt>:
  24:	55                   	push   %ebp
  25:	89 e5                	mov    %esp,%ebp
  
  The first 2 lines are know as prolog assembly function. It stores the previous base pointer (ebp) and set the base pointer as it was the top of the stack. This means that all the stack contents is saved down the stack, so the function can push/pop in the stack.

  27:	83 ec 08             	sub    $0x8,%esp
  2a:	83 ec 08             	sub    $0x8,%esp
  
The sub esp,12 line is saving space for local variables in the function.
  
  2d:	ff 75 08             	pushl  0x8(%ebp)

EBP points to the "bottom" of the stack frame. So when you need to put stuff into the the stack, you address it by subtracting from EBP.
  it takes the address of "EBP - 0x8"
  
  30:	68 00 00 00 00       	push   $0x0
  35:	e8 fc ff ff ff       	call   36 <geninterrupt+0x12>
  3a:	83 c4 10             	add    $0x10,%esp
  3d:	c6 05 45 00 00 00 05 	movb   $0x5,0x45

00000044 <genint>:
  44:	cd 00                	int    $0x0
  46:	90                   	nop
  47:	c9                   	leave  
  48:	c3                   	ret    

//*/

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[Hal.cpp]
//**
//****************************************************************************
