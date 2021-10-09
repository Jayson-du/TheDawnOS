#include "port.h"

/*=======================Port=======================*/
Port::Port(uint16_t Portnumber)
    : m_nPortNumber(Portnumber)
{}


Port::~Port()
{}


/*====================PortOf8Bit====================*/
PortOf8Bit::PortOf8Bit(uint16_t Portnumber)
    : Port(Portnumber){}

PortOf8Bit::~PortOf8Bit(){}

/*
*   inb (Bit, Port):   从I/O端口读取一个字节
*   outb(Bit, Port):   向I/O端口写入一个字节
*/
void PortOf8Bit::Write(uint8_t data)
{
    /*从data读取一个字节写入m_nPortNumber*/
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (m_nPortNumber));
}

uint8_t PortOf8Bit::Read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd"(m_nPortNumber));
    return result;
}


/*====================PortOf8BitSlow====================*/
PortOf8BitSlow::PortOf8BitSlow(uint16_t Portnumber)
    : PortOf8Bit(Portnumber)
{}

PortOf8BitSlow::~PortOf8BitSlow()
{}

void PortOf8BitSlow::Write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmP 1f\n1: jmP 1f\n1:" : : "a" (data), "Nd" (m_nPortNumber));
}

/*====================PortOf16Bit====================*/
PortOf16Bit::PortOf16Bit(uint16_t Portnumber)
    : Port(Portnumber)
{}

PortOf16Bit::~PortOf16Bit()
{}

/*
*   inw (Word, Port):   从I/O端口读取一个字(WORD,两个字节)
*   outw(Word, Port):   向I/O端口写入一个字(WORD,两个字节)
*/
void PortOf16Bit::Write(uint16_t data)
{
    /*从data读取一个字写入m_nPortNumber*/
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (m_nPortNumber));
}

uint16_t PortOf16Bit::Read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd"(m_nPortNumber));
    return result;
}


/*====================PortOf32Bit====================*/
PortOf32Bit::PortOf32Bit(uint16_t Portnumber)
    : Port(Portnumber)
{}

PortOf32Bit::~PortOf32Bit()
{}

/*
*   inl (Word, Port):   从I/O端口读取32位数据
*   outl(Word, Port):   向I/O端口写入32位数据
*/
void PortOf32Bit::Write(uint32_t data)
{
    /*从data读取一个32位数据写入m_nPortNumber*/
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (m_nPortNumber));
}

uint32_t PortOf32Bit::Read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd"(m_nPortNumber));
    return result;
}
