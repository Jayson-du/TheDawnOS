#include "hardwarecommunication/pci.h"

using namespace TDOS::Common;
using namespace TDOS::HardWareCommunication;
using namespace TDOS::Drivers;

void printf(const char* str);
void printfHex(uint8_t date);

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
                | ((nDevice & 0x1f) << 11) 
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

void PeripheralComponentInterconnectController::SelectDrivers(Drivers::DriverManager* drivermanager) 
{
    for(uint16_t bus = 0; bus < 256; bus++)
    {
        for(uint8_t device = 0; device < 32;device++)
        {
            uint16_t nFunctions = DeviceHasFunction((uint8_t)bus, device) ? 8 : 1;
            for(uint8_t nfunc = 0; nfunc < nFunctions; nfunc++)
            {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, nfunc);

                if(dev.nVenderID == 0 || dev.nVenderID == 0xffff)
                    break;
                
                printf("PCI BUS ");
                printfHex(bus & 0xff);

                printf(", DEVICE ");
                printfHex(device);

                printf(", FUNCTION ");
                printfHex(nfunc);

                printf(", VENDER ");
                printfHex((dev.nVenderID & 0xff00) >> 8);
                printfHex(dev.nVenderID & 0xff);

                printf(", DEVICE ");
                printfHex((dev.nDeviceID & 0xff00) >> 8);
                printfHex(dev.nDeviceID & 0xff);

                printf("\n");
            }
        }
    }
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(Common::uint8_t nbus,
                                                                                                               Common::uint8_t ndevice,
                                                                                                               Common::uint8_t nfunction) 
{
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.nBus = nbus;
    
    result.nDevice = ndevice;
    
    result.nFunction = nfunction;

    result.nVenderID = Read(nbus, ndevice, nfunction, 0);
    
    result.nDeviceID = Read(nbus, ndevice, nfunction, 0x02);

    result.nClassID = Read(nbus, ndevice, nfunction, 0x0b);
    
    result.nSubClassID = Read(nbus, ndevice, nfunction, 0x0a);
    
    result.nInterfaceID = Read(nbus, ndevice, nfunction, 0x09);
    
    result.nReversion = Read(nbus, ndevice, nfunction, 0x08);
    
    result.nInterrupt = Read(nbus, ndevice, nfunction, 0x3c);

    return result;
}