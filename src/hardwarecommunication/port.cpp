#include "hardwarecommunication/port.h"

using namespace TDOS::Common;
using namespace TDOS::HardWareCommunication;

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

void PortOf8Bit::Write(uint8_t data)
{
    Write8Bit(data, m_nPortNumber);
}

uint8_t PortOf8Bit::Read()
{
    Read8Bit(m_nPortNumber);
}


/*====================PortOf8BitSlow====================*/
PortOf8BitSlow::PortOf8BitSlow(uint16_t Portnumber)
    : PortOf8Bit(Portnumber)
{}

PortOf8BitSlow::~PortOf8BitSlow()
{}

void PortOf8BitSlow::Write(uint8_t data)
{
    Write8BitSlow(data, m_nPortNumber);
}

/*====================PortOf16Bit====================*/
PortOf16Bit::PortOf16Bit(uint16_t Portnumber)
    : Port(Portnumber)
{}

PortOf16Bit::~PortOf16Bit()
{}

void PortOf16Bit::Write(uint16_t data)
{
    Write16Bit(data, m_nPortNumber);
}

uint16_t PortOf16Bit::Read()
{
    Read16Bit(m_nPortNumber);
}


/*====================PortOf32Bit====================*/
PortOf32Bit::PortOf32Bit(uint16_t Portnumber)
    : Port(Portnumber)
{}

PortOf32Bit::~PortOf32Bit()
{}

void PortOf32Bit::Write(uint32_t data)
{
    Write32Bit(data, m_nPortNumber);
}

uint32_t PortOf32Bit::Read()
{
    Read32Bit(m_nPortNumber);
}
