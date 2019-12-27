#ifndef _DEBUGDISPLAY_H
#define _DEBUGDISPLAY_H
//****************************************************************************
//**
//**    DebugDisplay.h
//**    - Allows displaying debugging information for testing without
//**	worry of compatability nor hardware abstraction.
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================
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

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
extern void DebugUpdateCur (int x, int y);
extern void scroll () ;

extern void DebugPutc (unsigned char c);
extern void DebugClrScr (const uint8_t c);
extern void DebugPuts (char* str);
extern int DebugPrintf (const char* str, ...);
extern unsigned DebugSetColor (const unsigned c);
extern void DebugGotoXY (unsigned x, unsigned y);
extern void DebugGetXY (unsigned* x, unsigned* y);
extern int DebugGetHorz ();
extern int DebugGetVert ();

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
//**    END DebugDisplay.h
//**
//****************************************************************************
#endif
