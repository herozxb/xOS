#ifndef _HAL_H
#define _HAL_H
//****************************************************************************
//**
//**    Hal.h
//**		Hardware Abstraction Layer Interface
//**
//**	The Hardware Abstraction Layer (HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//**	All routines and types are declared extern and must be defined within
//**	external libraries to define specific hal implimentations.
//**
//****************************************************************************
#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#ifdef _MSC_VER
#define interrupt __declspec (naked)
#else
#define interrupt
#endif

#define far
#define near

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! initialize hardware abstraction layer
extern	int				hal_initialize ();

//! shutdown hardware abstraction layer
extern	int				hal_shutdown ();

//! generates interrupt
extern	void			geninterrupt (int n);


//! notifies hal interrupt is done
extern	void				interruptdone (unsigned int intno);

//! output sound to speaker
extern	void				sound (unsigned frequency);

//! read byte from device using port mapped io
extern	unsigned char	 inportb (unsigned short portid);

//! write byte to device through port mapped io
extern void				 outportb (unsigned short portid, unsigned char value);

// enable all hardware interrupts
static inline void enable () {
	__asm__ volatile (
	        ".intel_syntax noprefix \n\t"
	        "sti\n\r"
	        ".att_syntax");
}

// disable all hardware interrupts
static inline void disable () {
	__asm__ volatile (
	        ".intel_syntax noprefix \n\t"
	        "cli\n\r"
	        ".att_syntax"
	        :::"memory");
}

//! sets new interrupt vector
//extern void				 setvect (int intno, void (_cdecl far &vect) ( ) );

//! returns current interrupt vector
extern void				( far * 	getvect (int intno)) ( );

//! returns cpu vender
extern const char*		 get_cpu_vender ();

//! returns current tick count (only for demo)
extern int				 get_tick_count ();

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
//**    END [Hal.h]
//**
//****************************************************************************
#endif
