
#ifndef __PORT_H
#define __PORT_H

#include "types.h"

static inline uint8_t Read8(uint16_t _port)
{
	uint8_t result;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
	return result;
}

static inline void Write8(uint16_t _port, uint8_t _data)
{
	__asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
}

static inline void Write8Slow(uint16_t _port, uint8_t _data)
{
	__asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
}

static inline uint16_t Read16(uint16_t _port)
{
	uint16_t result;
	__asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
	return result;
}

static inline void Write16(uint16_t _port, uint16_t _data)
{
	__asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
}

static inline uint32_t Read32(uint16_t _port)
{
	uint32_t result;
	__asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
	return result;
}

static inline void Write32(uint16_t _port, uint32_t _data)
{
	__asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
}





#endif
