#ifndef __TDOS__PCI_H
#define __TDOS__PCI_H

/*         
*   @brief          PCI总线处理         
*   @author         dute  
*/

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/interrupts.h"
#include "drivers/driver.h"

namespace TDOS
{
    namespace HardWareCommunication
    {
        
        /*
        *                                                     PCI总线控制器读取的16个寄存器内容
        *            -------------------------------------------------------------------------------------------------------------------------
        *            |   register   |    offset   |   bits 31 - 24    |    bits 23 - 16    |    bits 15 - 8     |    bit 7 - 0               |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      00      |      00     |              Device ID                 |                Vendor ID                        |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      01      |      04     |              Status                    |                Command                          |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      02      |      08     |    Class Code     |    Subclass        |     Prog IF        |     Revision ID            |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      03      |      0C     |    BIST           |    Header Type           Latency Timer  |     Cache Line Size        |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      04      |      10     |                             Base address #0 (BAR0)                                       |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      05      |      14     |                             Base address #1 (BAR1)                                       |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      06      |      18     |                             Base address #2 (BAR2)                                       |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      07      |      1C     |                             Base address #3 (BAR3)                                       |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      08      |      20     |                             Base address #4 (BAR4)                                       |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      09      |      24     |                             Base address #5 (BAR5)                                       |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      0A      |      28     |                             Cardbus CIS Pointer                                          |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      0B      |      2C     |             Subsystem ID               |             Subsystem Vendor ID                 |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      0C      |      30     |                      Expansion ROM base address                                          |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      0D      |      34     |                      Reserved                                |    Capabilities Pointer   |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      0E      |      38     |                             Reserved                                                     |
        *            |-----------------------------------------------------------------------------------------------------------------------|
        *            |      0F      |      3C     |   Max latency     |     Min Grant      |     Interrupt PIN   |    Interrupt Line         |
        *            -------------------------------------------------------------------------------------------------------------------------
        */

        /************************************
        * @struct		PeripheralComponentInterconnectDeviceDescriptor
        * @brief		存储PCI数据的结构体
        * @author	 	durz
        ************************************/
        struct PeripheralComponentInterconnectDeviceDescriptor
        {
            Common::uint32_t nPortBase;
            Common::uint32_t nInterrupt;                  //中断

            Common::uint8_t  nBus;
            Common::uint8_t  nDevice;
            Common::uint8_t  nFunction;

            Common::uint16_t nDeviceID;
            Common::uint16_t nVenderID;

            Common::uint8_t  nClassID;
            Common::uint8_t  nSubClassID;
            Common::uint8_t  nInterfaceID;
            Common::uint8_t  nReversion;
        };

        /*
        *                                                   PCI总线控制器
        *    ------------------------------------------------------------------------------------------------------
        *    |      31      |   30 - 24  |   23 - 16    |    15 - 11      |     10 - 8        |      7 - 0        |
        *    ______________________________________________________________________________________________________   
        *    |  Enable Bit  |  Reserved  |  Bus Number  |  Device Number  |  Function Number  |  Register Offset  |
        *    ------------------------------------------------------------------------------------------------------
        *
        */

        class PeripheralComponentInterconnectController
        {
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            /*           
            *	@brief     读取总线上指令,PCI总共有256跟总线,每根总线挂在32个设备,每个设备有8种回调函数;
            *              PCI总线控制器根据第nbus跟总线,第nDevice个设备的第nfunctions回调函数 + 偏移值,
            *              计算数据端口应该获取哪一个PCI寄存器(32位)
            *	@param[in] nbus                 总线
            *	@param[in] nDevice              设备
            *	@param[in] nfunctions           回调函数
            *	@param[in] registeroffset       偏移的字节数(8位)

            */
            Common::uint32_t Read(Common::uint8_t nbus, 
                                  Common::uint8_t nDevice, 
                                  Common::uint8_t nfunctions,
                                  Common::uint8_t registeroffset);

            /*           
            *	@brief     读取总线上指令,PCI总共有256跟总线,每根总线挂在32个设备,每个设备有8种回调函数
            *	@param[in] nbus                 总线
            *	@param[in] nDevice              设备
            *	@param[in] nfunctions           回调函数
            *	@param[in] registeroffset       PCI总线控制器根据第nbus跟总线,第nDevice个设备的第nfunctions回调函数 + 偏移值计算内存位置
            */
            void Write(Common::uint8_t nbus, 
                                   Common::uint8_t  nDevice, 
                                   Common::uint8_t  nfunctions,
                                   Common::uint8_t  registeroffset,
                                   Common::uint32_t nData);

            /*           
            *	@brief     判断设备是否有回调函数
            *	@param[in] nbus                 总线
            *	@param[in] nDevice              设备
            */
            bool DeviceHasFunction(Common::uint8_t bus, Common::uint8_t device);

            void SelectDrivers(Drivers::DriverManager* drivermanager);

            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(Common::uint8_t nbus,
                                                                                Common::uint8_t ndevice,
                                                                                Common::uint8_t nfunction);
        private:
            PortOf32Bit m_nDataPort;                //数据端口
            PortOf32Bit m_nCommandPort;             //命令端口
        };                         
    }
}

#endif