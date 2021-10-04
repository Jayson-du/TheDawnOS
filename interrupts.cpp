#include "interrupts.h"

void printf(const char* str);

GateDescriptor InterruptManager::interruptDescriptorTable[256];


void InterruptManager::SetInterruptDescriptorTableEntry
(
    uint8_t     interruptNumber,
    uint16_t    codeSegmentSelectorOffset,
    void        (*handler)(),
    uint8_t     DescriptorPrivilegelLevel,
    uint8_t     DescriptorType
) 
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    //取低位两个字节
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xffff;
    //右移16位后取,取两个字节内容
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xffff;
    //偏移地址
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    //设置访问权限
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegelLevel & 3) << 5) | DescriptorType;

    interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(uint16_t nHardWareInterruptOffset, GlobalDescriptorTable* gdt)
    : m_nHardWareInterruptOffset(nHardWareInterruptOffset)
    , m_oPicMasterCommand(0x20)
    , m_oPicMasterData(0x21)
    , m_oPicSlaveCommand(0xA0)
    , m_oPicSlaveData(0xA1)
{
    uint16_t codeSegment = (gdt->CodeSegmentSelector()) << 3;           //这里忘记写 << 3

    const uint8_t IDT_INTERRUPT_GATE = 0xe;

    for(uint16_t i = 0;i < 256;i++)
    {
        SetInterruptDescriptorTableEntry(i, codeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    }

    DealWithException(codeSegment, IDT_INTERRUPT_GATE);
    DealWithInterrupts(codeSegment, IDT_INTERRUPT_GATE);

    //初始化8259A芯片
    m_oPicMasterCommand.write(0x11);                        //对于x86架构,初始化时应该写入"00010001 = 0x11"
    m_oPicSlaveCommand.write(0x11);
    
    m_oPicMasterData.write(nHardWareInterruptOffset);
    m_oPicSlaveData.write(nHardWareInterruptOffset + 8);

    m_oPicMasterData.write(0x04);
    m_oPicSlaveData.write(0x02);

    m_oPicMasterData.write(0x01);
    m_oPicSlaveData.write(0x01);

    //不屏蔽任何中断
    m_oPicMasterData.write(0x00);
    m_oPicSlaveData.write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    asm volatile("lidt %0": :"m" (idt));                    //取idt内存地址
}

InterruptManager::~InterruptManager(){}

void InterruptManager::Activate()
{
    asm("sti");                                             //开启中断
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp) 
{
    printf("interrupts");
    return esp;
}


void InterruptManager::DealWithInterrupts(uint16_t& codeSegment, const uint8_t& IDT_INTERRUPT_GATE)
{
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x00, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x01, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x02, codeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x03, codeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x04, codeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x05, codeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x06, codeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x07, codeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x08, codeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x09, codeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x0A, codeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x0B, codeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x0C, codeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x0D, codeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x0E, codeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x0F, codeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_nHardWareInterruptOffset + 0x31, codeSegment, &HandleInterruptRequest0x31, 0, IDT_INTERRUPT_GATE);
}

void InterruptManager::DealWithException(uint16_t& codeSegment, const uint8_t& IDT_INTERRUPT_GATE)
{

    SetInterruptDescriptorTableEntry(0x00, codeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, codeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, codeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, codeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, codeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, codeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, codeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, codeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, codeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, codeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, codeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, codeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, codeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, codeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, codeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, codeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, codeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, codeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, codeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, codeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

}
