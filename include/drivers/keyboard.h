#ifndef __TDOS__KEYBOARD_H
#define __TDOS__KEYBOARD_H

/*         
*   @fileName	keyboard.h
*   @brief    	键盘驱动
*   @author	    dute
*/

#include "hardwarecommunication/interrupts.h"
#include "drivers/driver.h"

namespace TDOS
{
    namespace Drivers
    {
        /*         
        *   @className KeyBoardEventHandler
        *   @brief     处理按键事件 
        */
        class KeyBoardEventHandler
        {
        public:
            KeyBoardEventHandler();

            /*           
            *	@brief     按键按下事件,外部可重写该事件
            *	@param[in]      key              被按下的键
            */
            virtual void OnKeyDown(char key);

            /*           
            *	@brief     按键抬起事件,外部可重写该事件
            *	@param[in]      key              被抬起的键
            */
            virtual void OnKeyUp(char key);
        };

        /*         
        *   @className KeyBoardDriver
        *   @brief     处理键盘输入 
        */
        class KeyBoardDriver : public IDriver, public HardWareCommunication::InterruptHandler
        {
        public:
            KeyBoardDriver(HardWareCommunication::InterruptManager* manager, KeyBoardEventHandler* handler);
            ~KeyBoardDriver();

            /*           
            *	@brief          键盘输入中断
            *	@param[in]      esp              堆栈栈顶
            *	@return    
            */
            virtual Common::uint32_t HandleInterrupt(Common::uint32_t esp);

            /*           
            *	@brief     激活键盘驱动
            *	@param[in]   
            */
            virtual void Activate();

        private:
            HardWareCommunication::PortOf8Bit m_oDataPort;                //数据端口
            HardWareCommunication::PortOf8Bit m_oCommandPort;             //命令端口

            KeyBoardEventHandler* m_pHandler;      //键盘事件
        };
    }
}

#endif