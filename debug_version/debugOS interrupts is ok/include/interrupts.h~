
#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H

#include "globalDescriptorTable.h"
#include "types.h"
#include "port.h"
#include "print.h"

struct GateDescriptor
{
	uint16_t handlerAddressLowBits;
	uint16_t gdt_codeSegmentSelector;
	uint8_t reserved;
	uint8_t access;
	uint16_t handlerAddressHighBits;
} __attribute__((packed));

struct InterruptDescriptorTablePointer
{
	uint16_t size;
	uint32_t base;
} __attribute__((packed));

//uint16_t hardwareInterruptOffset;

void Activate();
void InterruptIgnore();

void HandleInterruptRequest0x00();
void HandleInterruptRequest0x01();
void HandleInterruptRequest0x02();
void HandleInterruptRequest0x03();
void HandleInterruptRequest0x04();
void HandleInterruptRequest0x05();
void HandleInterruptRequest0x06();
void HandleInterruptRequest0x07();
void HandleInterruptRequest0x08();
void HandleInterruptRequest0x09();
void HandleInterruptRequest0x0A();
void HandleInterruptRequest0x0B();
void HandleInterruptRequest0x0C();
void HandleInterruptRequest0x0D();
void HandleInterruptRequest0x0E();
void HandleInterruptRequest0x0F();
void HandleInterruptRequest0x31();

void HandleException0x00();
void HandleException0x01();
void HandleException0x02();
void HandleException0x03();
void HandleException0x04();
void HandleException0x05();
void HandleException0x06();
void HandleException0x07();
void HandleException0x08();
void HandleException0x09();
void HandleException0x0A();
void HandleException0x0B();
void HandleException0x0C();
void HandleException0x0D();
void HandleException0x0E();
void HandleException0x0F();
void HandleException0x10();
void HandleException0x11();
void HandleException0x12();
void HandleException0x13();

extern "C" uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);
//*/


void setInterruptDescriptorTableEntry( GateDescriptor* interruptDescriptorTable, uint8_t interrupt, uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType);



void interruptManager(uint16_t hardwareInterruptOffset_in, GlobalDescriptorTable* globalDescriptorTable, GateDescriptor* interruptDescriptorTable);

//*/

#endif
