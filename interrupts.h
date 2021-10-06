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
*   @className  InterruptHandler
*   @brief      处理中断基类
*/

class InterruptHandler;

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

struct InterruptDescriptorTablePointer
{
    uint16_t size;
    uint32_t base;
}__attribute__((packed));


/*         
*   @className  InterruptManager
*   @brief      中断管理器
*/
class InterruptManager
{
    friend class InterruptHandler;
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

    uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

    /*           
    *	@brief     CPU开启中断
    */
    void Activate();
    
    /*           
    *	@brief     CPU关闭中断
    */
    void Deactivate();

protected:
    static GateDescriptor interruptDescriptorTable[256];

    static InterruptManager* spActiveInterruptManager;

    static void SetInterruptDescriptorTableEntry
    (
        uint8_t     interruptNumber,
        uint16_t    codeSegmentSelectorOffset,
        void        (*handler)(),
        uint8_t     DescriptorPrivilegelLevel,
        uint8_t     DescriptorType
    );

    static void InterruptIgnore();

    uint16_t m_nHardWareInterruptOffset;

    InterruptHandler* m_handlerarray[256];

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

protected:
    /*           
    *	@brief     处理异常
    */
    void DealWithException(uint16_t& codeSegment, const uint8_t& IDT_INTERRUPT_GATE);

    /*           
    *	@brief     处理中断
    */
    void DealWithInterrupts(uint16_t& codeSegment, const uint8_t& IDT_INTERRUPT_GATE);

private:
    /*
    *8259A中断芯片通过两个I/O地址来进行中断相关的数据传输,告诉CPU发生了中断
    *通常情况下是单个使用或者两级级连(个人理解是两个并联)
    *使用单个8259A芯片,传输的I/O端口是0x20和0x21
    *使用两级级连,传输的I/O端口是0x10和0x11
    *两级级连时,会区分一个主中断与一个从中断
    */
    //主中断
    PortOf8BitSlow m_oPicMasterCommand;
    PortOf8BitSlow m_oPicMasterData;
    //从中断
    PortOf8BitSlow m_oPicSlaveCommand;
    PortOf8BitSlow m_oPicSlaveData;
};

class InterruptHandler
{
public:
    virtual uint32_t HandleInterrupt(uint32_t esp);

public:
    InterruptHandler(uint8_t interruptNumber, InterruptManager* pInterruptManager);

    /*
    *	@brief     因为InterruptManager与InterruptHandler相互引用,所以构造函数与析构函数不能为virtual,
    *              否则编译会提示
    *              interrupts.o: In function `__ZN16InterruptHandlerD0Ev':
    *              interrupts.cpp:(.text+0xb30): undefined reference to `__ZdlPvj'
    *              跟编译原理相关,暂时先记一下,后续需要查看编译原理
    */
    //virtual ~InterruptHandler();      //error,编译会报错
    ~InterruptHandler();                //correct,编译正常通过
    
    uint8_t m_nInterruptNumber;
    InterruptManager* m_pInterruptManager;

};

#endif
