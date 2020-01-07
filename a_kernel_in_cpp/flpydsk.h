
#ifndef _FLPYDSK_DRIVER_H
#define _FLPYDSK_DRIVER_H
//****************************************************************************
//**
//**    flpydsk.h
//**
//****************************************************************************

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif



#include <stdint.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
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

//! install floppy driver
extern void flpydsk_install (int irq);

//! set current working drive
extern void flpydsk_set_working_drive (uint8_t drive);

//! get current working drive
extern uint8_t flpydsk_get_working_drive ();

//! read a sector
extern uint8_t* flpydsk_read_sector (int sectorLBA);

//! converts an LBA address to CHS
extern void flpydsk_lba_to_chs (int lba,int *head,int *track,int *sector);

extern void i86_flpy_irq_handler_main(void);
extern void i86_flpy_irq ();

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [flpydsk.h]
//**
//****************************************************************************

#ifdef __cplusplus
}
#endif

#endif
