#ifndef _GDT_H
#define _GDT_H

#include "types.h"

class SegmentDescriptor
{
public:
    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
    
    uint32_t Base();
    uint32_t Limit();

private:
    uint16_t limit;
    uint16_t base_lo;
    uint8_t base_hi;
    uint8_t type;
    uint8_t flags_limits;
    uint8_t base_vhi;
}__attribute__((packed));


class GlobalDescriptorTable
{
public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegmentDescriptor();
    uint16_t DataSegmentDescriptor();

    SegmentDescriptor nullSegmentDescriptor;
    SegmentDescriptor unusedSegmentDescriptor;
    SegmentDescriptor codeSegmentDescriptor;
    SegmentDescriptor dataSegmentDescriptor;


};

#endif