#ifndef __TDOS__PCI_H
#define __TDOS__PCI_H

/*         
*   @brief          PCI总线处理         
*   @author         dute  
*/

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/interrupts.h"

namespace TDOS
{
    namespace HardWareCommunication
    {

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
        private:
            PortOf32Bit m_nDataPort;                //数据端口
            PortOf32Bit m_nCommandPort;             //命令端口
        };               
    }
}

#endif