
//****************************************************************************
//**
//**    tss.h
//**
//**	Task State Segment
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "string.h"
#include "gdt.h"
#include "tss.h"
#include "Hal.h"

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

static tss_entry TSS;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

void install_tsr (uint16_t sel);

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

void flush_tss (uint16_t sel) {

//	_asm {
//		cli
//		mov eax, 0x2b
//		ltr eax
//		sti
//	}
	
	__asm__ volatile (
		  ".intel_syntax noprefix \n\t"
			"ltr ax\n\t"
			".att_syntax"
			:
			:"a"(sel << 3)
			:
			);
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

void tss_set_stack (uint16_t kernelSS, uint16_t kernelESP) {

	TSS.ss0 = kernelSS;
	TSS.esp0 = kernelESP;
}

void install_tss (uint32_t idx, uint16_t kernelSS, uint16_t kernelESP) {

	//! install TSS descriptor
	uint32_t base = (uint32_t) &TSS;

	//! install descriptor
	gdt_set_descriptor (idx, base, base + sizeof (tss_entry),
		I86_GDT_DESC_ACCESS|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_DPL|I86_GDT_DESC_MEMORY,
		0);

	//! initialize TSS
	memset ((void*) &TSS, 0, sizeof (tss_entry));

	//! set stack and segments
	TSS.ss0 = kernelSS;
	TSS.esp0 = kernelESP;
	TSS.cs=0x0b;
	TSS.ss = 0x13;
	TSS.es = 0x13;
	TSS.ds = 0x13;
	TSS.fs = 0x13;
	TSS.gs = 0x13;

	//! flush tss
	flush_tss (idx);
	//flush_tss (idx * sizeof (gdt_descriptor));
}



void go_user () {

    printf ("------[1.0]------\n");
    //	int stack=0;
    //	_asm mov [stack], esp
/*

    int stack=0;
    __asm__ __volatile__
    (  // ".intel_syntax noprefix \n\t"
	      ".intel_syntax noprefix \n\t" 
        "mov [%[stack]], esp\n\t"
        ".att_syntax"
        :  [stack] "=&r" (stack)
        :
    );
    printf ("------[1.1]------\n");
    extern void tss_set_stack (uint16_t, uint16_t);
    tss_set_stack (0x10,stack);

    printf ("------[1.2]------\n");
    enter_usermode();
    printf ("------[1.3]------\n");

    char testStr[]="\n\rWe are inside of your computer...";

//! call OS-print message
//	_asm xor eax, eax
//	_asm lea ebx, [testStr]
//	_asm int 0x80
	//*/
	  geninterrupt(0x80);
	  /*
	  			__asm__ volatile (
	        ".intel_syntax noprefix \n\t"
	        "int 0x80\n\r"
	        ".att_syntax"
	        );
/*		
			__asm__ volatile (
	        ".intel_syntax noprefix \n\t"
	        "xor eax, eax\n\r"
	        "lea ebx, [%[testStr]]\n\r"
	        "int 0x80\n\r"
	        ".att_syntax"
	        :: [testStr] "g" (testStr)
	        );
	        //*/
		printf ("------We are inside of your computer...------\n");
	//! cant do CLI+HLT here, so loop instead
	while(1);
}



/**
*	Enters user mode
*/
//void enter_usermode () {

/*
	_asm {

		cli
		mov ax, 0x23	; user mode data selector is 0x20 (GDT entry 3). Also sets RPL to 3
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax

		push 0x23		; SS, notice it uses same selector as above
		push esp		; ESP
		pushfd			; EFLAGS

		pop eax
		or eax, 0x200	; enable IF in EFLAGS
		push eax

		push 0x1b		; CS, user mode code selector is 0x18. With RPL 3 this is 0x1b
		lea eax, [a]	; EIP first
		push eax
		iretd
	a:
		add esp, 4 // fix stack
	}
	
//*/
	/*
		
		__asm__ volatile (
	        ".intel_syntax noprefix \n\t"
	        "cli\n\r"
	        "mov ax, 0x23\n\r"
	        "mov ds, ax\n\r"
	        "mov es, ax\n\r"
	        "mov fs, ax\n\r"
	        "mov gs, ax\n\r"
	        "push 0x23\n\r"
	        "push esp\n\r"	        
	        "pushfd\n\r"
	        "pop eax\n\r"
	        "or eax, 0x200\n\r"
	        "push eax\n\r"	        
	        "push 0x1b\n\r"
	        "lea eax, [a]\n\r"
	        "push eax\n\r"
	        "iretd\n\r"
	        "a:\n\r"
	        "add esp, 4\n\r"
	        ".att_syntax");
}
//*/
//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[tss.cpp]
//**
//****************************************************************************
