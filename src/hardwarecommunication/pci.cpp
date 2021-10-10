#include "hardwarecommunication/pci.h"

using namespace TDOS::Common;
using namespace TDOS::HardWareCommunication;

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    : m_nDataPort(0xCFC)                    //数据端口地址
    , m_nCommandPort(0xCF8)                 //命令端口地址
{

}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController() 
{
    
}

uint32_t PeripheralComponentInterconnectController::Read(uint8_t nbus, 
                                                         uint8_t nDevice, 
                                                         uint8_t nfunctions,
                                                         uint8_t registeroffset) 
{
    //计算硬件地址
    uint32_t nID = (1 << 31) 
                | ((nbus & 0xff) << 16) 
                | ((nDevice & 0x1f) < 11) 
                | ((nfunctions & 0x07) << 8)
                | (registeroffset & 0xfc);
    
    m_nCommandPort.Write(nID);                      //写入设备地址
    uint32_t result = m_nDataPort.Read();           //从指定Id的寄存器(假设为第n个)读取32位数据

    //此时result是32位数据,需要根据offset获取想要的数据;
    //因为registeroffset表示偏移的字节数,
    //所以对于一个32为数据来说,registeroffset % 4表示将前面n - 1个寄存器的数据都偏移了
    //8 * (registeroffset % 4)表示第n个寄存器还需要偏移(8 * (registeroffset % 4))位
    return result >> (8 * (registeroffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint8_t   nbus, 
                                                      uint8_t   nDevice, 
                                                      uint8_t   nfunctions,
                                                      uint8_t   registeroffset,
                                                      uint32_t  nData)
{
        //计算硬件地址
    uint32_t nID = (1 << 31) 
                | ((nbus & 0xff) << 16) 
                | ((nDevice & 0x1f) < 11) 
                | ((nfunctions & 0x07) << 8)
                | (registeroffset & 0xfc);
    
    m_nCommandPort.Write(nID);                      //写入设备地址
    
    m_nDataPort.Write(nData);
}

bool PeripheralComponentInterconnectController::DeviceHasFunction(uint8_t bus, uint8_t device) 
{
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}
