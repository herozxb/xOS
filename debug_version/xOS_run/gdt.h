#ifndef __GDT_H
#define __GDT_H

    #include "types.h"
    
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor //subclass
            {
                private: //all have 8 bytes
                    uint16_t limit_lo;  //low bytes 2 bytes
                    uint16_t base_lo;   //2 bytes
                    uint8_t base_hi;    //1 bytes
                    uint8_t type; 	//1 bytes
                    uint8_t limit_hi; 	//1 bytes
                    uint8_t base_vhi; 	//1 bytes

                public:
                    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                    uint32_t Base();
                    uint32_t Limit();
            } __attribute__((packed)); // for complier to packed the code

        private:
            SegmentDescriptor nullSegmentSelector;	//8 bytes
            SegmentDescriptor unusedSegmentSelector;	//8 bytes
            SegmentDescriptor codeSegmentSelector;	//8 bytes
            SegmentDescriptor dataSegmentSelector;	//8 bytes

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            uint16_t CodeSegmentSelector();		//2 bytes
            uint16_t DataSegmentSelector();		//2 bytes
    };

#endif
