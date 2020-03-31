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


//! DMA Routines provided for driver use
extern	void dma_set_mode (uint8_t channel, uint8_t mode);
extern	void dma_set_read (uint8_t channel);
extern	void dma_set_write (uint8_t channel);
extern	void dma_set_address(uint8_t channel, uint8_t low, uint8_t high);
extern	void dma_set_count(uint8_t channel, uint8_t low, uint8_t high);
extern	void dma_mask_channel (uint8_t channel);
extern	void dma_unmask_channel (uint8_t channel);
extern	void dma_reset_flipflop (int dma);
extern  void dma_enable (uint8_t ctrl, bool e);
extern  void dma_reset (int dma);
extern  void dma_set_external_page_register (uint8_t reg, uint8_t val);
extern  void dma_unmask_all (int dma);



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
