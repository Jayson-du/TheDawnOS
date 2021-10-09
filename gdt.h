#ifndef _GDT_H
#define _GDT_H

/*
* GDT(Global Descriptor Table)全局描述符表,在整个系统中,全局描述符表只有一张(一个处理器对应一个GDT),GDT可以被放在内存的任何位置
* ,但CPU必须知道GDT的入口,也就是基地址存放在哪里;Intel的设计者提供了一个寄存器GDTR用来存放GDT的入口地址,程序员将GDT设定在内存中的
* 某个位置之后,可以通过LGDT指令将GDT的入口地址装入次寄存器当中;CPU就根据此寄存器中的内容作为GDT的入口来访问GDT了；GDTR中存放的是GDT
* 在内存中的基地址和其表长界限；
*/

/*
*                                   GDT表内存分布图
*                ---------------------------------------------------------
*                |   属性(12位)      |    段基址(32位)    |    段界限(20位)  |
*                ---------------------------------------------------------
*
*   31                  24  23      22       21   20                  16   15  14   13   12   11    8  7                0
*    _____________________________________________________________________________________________________________________
*    |  段基地址： 31 ~ 24  |  G  |  D/B  |  L  |  AVL  |  段界限:19 ~ 16  |  P  |  DPL  |  S  |  TYPE  |  段基地址:23 ~ 16  |
*    ---------------------------------------------------------------------------------------------------------------------
*
*
*    31                                                     16   15                                                      0
*    ______________________________________________________________________________________________________________________
*    |                   段基地址: 15 ~ 0                         |                  段界限：15 ~ 0                          |
*    ----------------------------------------------------------------------------------------------------------------------
*
*
*
*
* 全局描述符表结构
* base      :   基址(起始地址)
* limit     :   寻址最大范围
* flags     :   标志位,
* Type      :   标记什么段（数据段、代码段）
* access    :   访问权限
*/

/*
*       实模式（0 - 100000）物理地址直接暴露给程序员
*       保护模式(10FFEF - ~)物理地址不直接暴露给程序员
*寻址方式：
*       分段管理:   段基址 + 偏移地址   =>  线性地址
*       段值存入段寄存器，而该值作为索引，用于在GDT表中寻找到对应的一个表项（段描述符），该表项中还有段地址，段大小，访问控制等信息；
        得到其中的段地址后再加上合法的段内偏移，即可访问到对应的物理地址；
*       分页管理:   线性地址(通过页表)  =>  物理地址
*/

#include "types.h"


/*!
*   @class      SegmentDescriptor
*   @brief      段描述符
*/

class SegmentDescriptor
{
public:
    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
    
    uint32_t Base();
    uint32_t Limit();

private:
    uint16_t limit;                                         //两个字节构成的limit段
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

    /*!
    *@brief     获取代码段
    *@return    
    */
    uint16_t CodeSegmentSelector();

    /*!
    *@brief     获取数据段
    *@return    
    */
    uint16_t DataSegmentSelector();

    SegmentDescriptor nullSegmentDescriptor;
    SegmentDescriptor unusedSegmentDescriptor;
    SegmentDescriptor codeSegmentDescriptor;                //代码段
    SegmentDescriptor dataSegmentDescriptor;                //数据段


};

#endif