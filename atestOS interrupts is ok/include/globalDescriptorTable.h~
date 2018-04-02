#ifndef __GDT_H
#define __GDT_H

#include "types.h"

struct SegmentDescriptor //subclass
{
    uint16_t limit_lo;  //low bytes 2 bytes
    uint16_t base_lo;   //2 bytes
    uint8_t base_hi;    //1 bytes
    uint8_t type; 	//1 bytes
    uint8_t limit_hi; 	//1 bytes
    uint8_t base_vhi; 	//1 bytes

} __attribute__((packed)); // for complier to packed the code

void setupSegmentDescriptor(uint8_t* segmentPointer, uint32_t base, uint32_t limit, uint8_t type);



struct GlobalDescriptorTable
{
    SegmentDescriptor nullSegmentSelector;	//8 bytes
    SegmentDescriptor unusedSegmentSelector;	//8 bytes
    SegmentDescriptor codeSegmentSelector;	//8 bytes
    SegmentDescriptor dataSegmentSelector;
} __attribute__((packed));

void loadTable(GlobalDescriptorTable* gdtTable);



uint16_t dataSegmentSelector(GlobalDescriptorTable* gdtTable);


uint16_t codeSegmentSelector(GlobalDescriptorTable* gdtTable);


uint32_t Base(GlobalDescriptorTable* gdtTable);


uint32_t Limit(GlobalDescriptorTable* gdtTable);


#endif
