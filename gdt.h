#ifndef _GDT_H
#define _GDT_H

/*
* GDT(Global Descriptor Table)全局描述符表,在整个系统中,全局描述符表只有一张(一个处理器对应一个GDT),GDT可以被放在内存的任何位置
* ,但CPU必须知道GDT的入口,也就是基地址存放在哪里;Intel的设计者提供了一个寄存器GDTR用来存放GDT的入口地址,程序员将GDT设定在内存中的
* 某个位置之后,可以通过LGDT指令将GDT的入口地址装入次寄存器当中;
*/

/*
* 全局描述符表结构
* base      :   基址(注意,base的byte是分散开的)
* limit     :   寻址最大范围
* flags     :   标志位,
* access    :   访问权限
*/

#include "types.h"

class SegmentDescriptor
{
public:
    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
    
    uint32_t Base();
    uint32_t Limit();

private:
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_high;
    uint8_t type;
    uint8_t flags_limits;
    uint8_t base_vhi;
}__attribute__((packed));                                   //手动设置内存对齐大小


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