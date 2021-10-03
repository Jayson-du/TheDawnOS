#include "port.h"

/*=======================port=======================*/
port::port(uint16_t portnumber)
    : m_nPortNumber(portnumber)
{}


port::~port()
{}


/*====================portOf8Bit====================*/
portOf8Bit::portOf8Bit(uint16_t portnumber)
    : port(portnumber){}

portOf8Bit::~portOf8Bit(){}

/*
*   inb (Bit, port):   从I/O端口读取一个字节
*   outb(Bit, port):   向I/O端口写入一个字节
*/
void portOf8Bit::write(uint8_t data)
{
    /*从data读取一个字节写入m_nPortNumber*/
    __asm__ volatile("outb %0, %1" : "=a" (data) : "Nd" (m_nPortNumber));
}

uint8_t portOf8Bit::read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd"(m_nPortNumber));
    return result;
}


/*====================portOf8BitSlow====================*/
portOf8BitSlow::portOf8BitSlow(uint16_t portnumber)
    : portOf8Bit(portnumber)
{}

portOf8BitSlow::~portOf8BitSlow()
{}

void portOf8BitSlow::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : "=a" (data) : "Nd" (m_nPortNumber));
}

/*====================portOf16Bit====================*/
portOf16Bit::portOf16Bit(uint16_t portnumber)
    : port(portnumber)
{}

portOf16Bit::~portOf16Bit()
{}

/*
*   inw (Word, port):   从I/O端口读取一个字(WORD,两个字节)
*   outw(Word, port):   向I/O端口写入一个字(WORD,两个字节)
*/
void portOf16Bit::write(uint16_t data)
{
    /*从data读取一个字写入m_nPortNumber*/
    __asm__ volatile("outw %0, %1" : "=a" (data) : "Nd" (m_nPortNumber));
}

uint16_t portOf16Bit::read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd"(m_nPortNumber));
    return result;
}


/*====================portOf32Bit====================*/
portOf32Bit::portOf32Bit(uint16_t portnumber)
    : port(portnumber)
{}

portOf32Bit::~portOf32Bit()
{}

/*
*   inl (Word, port):   从I/O端口读取32位数据
*   outl(Word, port):   向I/O端口写入32位数据
*/
void portOf32Bit::write(uint32_t data)
{
    /*从data读取一个32位数据写入m_nPortNumber*/
    __asm__ volatile("outl %0, %1" : "=a" (data) : "Nd" (m_nPortNumber));
}

uint32_t portOf32Bit::read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd"(m_nPortNumber));
    return result;
}