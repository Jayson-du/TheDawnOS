#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

/*         
*   @fileName	interrupts.h
*   @brief      操作中断
*   @author     durz
*/
#include "types.h"
#include "port.h"
#include "gdt.h"

/*
*   @brief      
*/
struct GateDescriptor
{
    uint16_t handlerAddressLowBits;
    uint16_t gdt_codeSegmentSelector;
    uint8_t  reserved;
    uint8_t  access;
    uint16_t handlerAddressHighBits;
}__attribute__((packed));                                   //让编译器不进行内存对齐操作

/*         
*   @className  InterruptManager
*   @brief      中断管理器
*/
class InterruptManager
{
public:
    InterruptManager(uint16_t nHardWareInterruptOffset, GlobalDescriptorTable* gdt);
    ~InterruptManager();

    /*           
    *	@brief     处理中断函数
    *	@param[in] interruptNumber  中断号
    *	@param[in] esp              堆栈栈顶
    *   @return    处理玩中断后,返回堆栈栈顶
    */
    static uint32_t HandleInterrupt(uint8_t interruptNumber, uint32_t esp);

protected:
    static GateDescriptor interruptDescriptorTable[256];

    static void SetInterruptDescriptorTableEntry
    (
        uint8_t     interruptNumber,
        uint16_t    codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t     DescriptorPrivilegelLevel,
        uint8_t     DescriptorType
    );

    static void InterruptIgnore();

    uint16_t m_nHardWareInterruptOffset;

protected:
/*====================中断====================*/
    /*           
    *	@brief     中断请求号
    */
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

protected:
/*====================异常====================*/
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
};

#endif


