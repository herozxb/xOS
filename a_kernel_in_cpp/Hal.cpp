
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
#include "pic.h"
#include "pit.h"

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


	//! initialize motherboard controllers and system timer
	i86_cpu_initialize ();
	i86_pic_initialize (0x20,0x28);
	i86_pit_initialize ();
	i86_pit_start_counter (100,I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);

	//! enable interrupts
	enable ();


	return 0;
}

//! shutdown hardware devices
int hal_shutdown () {

	i86_cpu_shutdown ();
	return 0;
}

void geninterrupt(int n){
	__asm__ (
	    ".intel_syntax noprefix \n\t"
			"mov byte ptr [genint+1], al\n\t"
			"jmp genint\n"
			"genint:\n\t"
			"int 0\n\t" // above code modifies the 0 to int number to generate
			".att_syntax"
			:
			:"a"(n)
			);
}


//! notifies hal interrupt is done
inline void 	interruptdone (unsigned int intno) {

	//! insure its a valid hardware irq
	if (intno > 16)
		return;

	//! test if we need to send end-of-interrupt to second pic
	if (intno >= 8)
		i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 1);

	//! always send end-of-interrupt to primary pic
	i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 0);
}


//! output sound to speaker
void 	sound (unsigned frequency) {

	//! sets frequency for speaker. frequency of 0 disables speaker
	outportb (0x61, 3 | (unsigned char)(frequency<<2) );
}


//! read byte from device using port mapped io
unsigned char  inportb (unsigned short portid) {
	unsigned char result;
	__asm__ volatile (
	         ".intel_syntax noprefix \n\t" 
	         "in al, dx\n\t"
	         ".att_syntax"
					 :"=a"(result)
					 :"d"(portid)
					 );
	return result;
}


//! write byte to device through port mapped io
void  outportb (unsigned short portid, unsigned char value) {
	__asm__ volatile (
	         ".intel_syntax noprefix \n\t" 
	         "out dx, al\n\t"
	         ".att_syntax"
					 :
					 :"a"(value), "d"(portid)
					 );
}



//! sets new interrupt vector
//void  setvect (int intno, void ( far &vect) ( ) ) {

	//! install interrupt handler! This overwrites prev interrupt descriptor
//	i86_install_ir (intno, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
//		0x8, vect);
//}


//! returns current interrupt vector
void ( far * 	getvect (int intno)) ( ) {

	//! get the descriptor from the idt
	IDT_entry* desc = i86_get_ir (intno);
	if (!desc)
		return 0;

	//! get address of interrupt handler
	uint32_t addr = desc->offset_lowerbits | (desc->offset_higherbits << 16);

	//! return interrupt handler
	I86_IRQ_HANDLER irq = (I86_IRQ_HANDLER)addr;
	return irq;
}


//! returns cpu vender
const char*  get_cpu_vender () {

	return i86_cpu_get_vender();
}


//! returns current tick count (only for demo)
int  get_tick_count () {

	return i86_pit_get_tick_count();
}


//! generate interrupt call
/*
void geninterrupt (int n) {
//*/
/*
    DebugPrintf ("\n[0x%x]",n);

    
        asm volatile (
        "movb (%0), %%al  \n\t"
        "movb %%al, (genint+1) \n\t"
        "jmp genint  \n\t"
        "genint:\n\t"
        "int $0 \n\t"
        :
        : "r" (n)           
        );
        
        //*/
/*
    asm volatile (
        ".intel_syntax noprefix \n\t"

        "mov byte ptr [genint+1], 5\n\t" 
        "genint: int 0\n\t"
        ".att_syntax"

        );
//*/

/*
    asm volatile (
        ".intel_syntax noprefix \n\t"
        "mov byte ptr [genint+1], al\n\t" 
        "genint: int 0\n\t"
        ".att_syntax"
        ::"a"(n)    
        );
  //*/      
  /*
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
//*/

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
