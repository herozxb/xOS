#include "./include/interrupts.h"


/*
void InterruptIgnore()
{}


void HandleInterruptRequest0x00()
{
	printf("at the c file function");
}
//*/

void Activate()
{
    //if(ActiveInterruptManager == 0)
    {
        //ActiveInterruptManager = this;
        asm("sti");
	//CLI stop interrupt
	//STI begin interrupt
    }
}


extern "C" uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp)
{
    char* foo = "INTERRUPT 0x00";
    char* hex = "0123456789ABCDEF";

    foo[12] = hex[(interrupt >> 4) & 0xF];
    foo[13] = hex[interrupt & 0xF];
    printf(foo);

    return esp;
}

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

//*/

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

//*/
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

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(GateDescriptor) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
    printf("In the end of interrupt manager function\n");

}


