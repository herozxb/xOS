
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



static void InterruptIgnore();

static void HandleInterruptRequest0x00();
static void HandleInterruptRequest0x01();
static void HandleInterruptRequest0x02();
static void HandleInterruptRequest0x03();
static void HandleInterruptRequest0x04();
static void HandleInterruptRequest0x05();
static void HandleInterruptRequest0x06();
static void HandleInterruptRequest0x07();
static void HandleInterruptRequest0x08();
static void HandleInterruptRequest0x09();
static void HandleInterruptRequest0x0A();
static void HandleInterruptRequest0x0B();
static void HandleInterruptRequest0x0C();
static void HandleInterruptRequest0x0D();
static void HandleInterruptRequest0x0E();
static void HandleInterruptRequest0x0F();
static void HandleInterruptRequest0x31();

static void HandleException0x00();
static void HandleException0x01();
static void HandleException0x02();
static void HandleException0x03();
static void HandleException0x04();
static void HandleException0x05();
static void HandleException0x06();
static void HandleException0x07();
static void HandleException0x08();
static void HandleException0x09();
static void HandleException0x0A();
static void HandleException0x0B();
static void HandleException0x0C();
static void HandleException0x0D();
static void HandleException0x0E();
static void HandleException0x0F();
static void HandleException0x10();
static void HandleException0x11();
static void HandleException0x12();
static void HandleException0x13();

static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);

uint16_t hardwareInterruptOffset;

void setInterruptDescriptorTableEntry( GateDescriptor* interruptDescriptorTable, uint8_t interrupt, uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType)
{
    // address of pointer to code segment (relative to global descriptor table)
    // and address of the handler (relative to segment)
    interruptDescriptorTable[interrupt].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interrupt].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].reserved = 0;
}


void interruptManager(uint16_t hardwareInterruptOffset_in, GlobalDescriptorTable* globalDescriptorTable, GateDescriptor* interruptDescriptorTable)
{
    uint16_t hardwareInterruptOffset = hardwareInterruptOffset_in;
    uint32_t CodeSegment = codeSegmentSelector(globalDescriptorTable);

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint8_t i = 255; i > 0; --i)
    {
        setInterruptDescriptorTableEntry(interruptDescriptorTable,i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
        //handlers[i] = 0;
    }

    setInterruptDescriptorTableEntry(interruptDescriptorTable,0, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    //handlers[0] = 0;

    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x02, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x03, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x04, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x05, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x06, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x07, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x08, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x09, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x0A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x0B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x0D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x0E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(interruptDescriptorTable,hardwareInterruptOffset + 0x0F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

//*/

    //programmableInterruptControllerMasterCommandPort.Write(0x11);
	Write8Slow(0x20, 0x11);
    //programmableInterruptControllerSlaveCommandPort.Write(0x11);
	Write8Slow(0xA0, 0x11);
    // remap
    //programmableInterruptControllerMasterDataPort.Write(hardwareInterruptOffset);
	Write8Slow(0x21, hardwareInterruptOffset);
    //programmableInterruptControllerSlaveDataPort.Write(hardwareInterruptOffset+8);
	Write8Slow(0xA1, hardwareInterruptOffset+8);

    //programmableInterruptControllerMasterDataPort.Write(0x04);
	Write8Slow(0x21, 0x04);
    //programmableInterruptControllerSlaveDataPort.Write(0x02);
	Write8Slow(0xA1, 0x02);

    //programmableInterruptControllerMasterDataPort.Write(0x05);
	Write8Slow(0x21, 0x05);
    //programmableInterruptControllerSlaveDataPort.Write(0x01);
	Write8Slow(0xA1, 0x01);

    //programmableInterruptControllerMasterDataPort.Write(0x00);
	Write8Slow(0x21, 0x00);
    //programmableInterruptControllerSlaveDataPort.Write(0x00);
	Write8Slow(0xA1, 0x00);
    printf("@In now 2\n");
    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(GateDescriptor) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));

}



void InterruptIgnore()
{}

void HandleInterruptRequest0x00(){}
void HandleInterruptRequest0x01(){}
void HandleInterruptRequest0x02(){}
void HandleInterruptRequest0x03(){}
void HandleInterruptRequest0x04(){}
void HandleInterruptRequest0x05(){}
void HandleInterruptRequest0x06(){}
void HandleInterruptRequest0x07(){}
void HandleInterruptRequest0x08(){}
void HandleInterruptRequest0x09(){}
void HandleInterruptRequest0x0A(){}
void HandleInterruptRequest0x0B(){}
void HandleInterruptRequest0x0C(){}
void HandleInterruptRequest0x0D(){}
void HandleInterruptRequest0x0E(){}
void HandleInterruptRequest0x0F(){}
void HandleInterruptRequest0x31(){}

void HandleException0x00(){}
void HandleException0x01(){}
void HandleException0x02(){}
void HandleException0x03(){}
void HandleException0x04(){}
void HandleException0x05(){}
void HandleException0x06(){}
void HandleException0x07(){}
void HandleException0x08(){}
void HandleException0x09(){}
void HandleException0x0A(){}
void HandleException0x0B(){}
void HandleException0x0C(){}
void HandleException0x0D(){}
void HandleException0x0E(){}
void HandleException0x0F(){}
void HandleException0x10(){}
void HandleException0x11(){}
void HandleException0x12(){}
void HandleException0x13(){}

//*/

#endif
