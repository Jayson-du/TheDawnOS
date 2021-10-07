#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/*         
*   @fileName	keyboard.h
*   @brief    	键盘驱动
*   @author	    dute
*/

#include "interrupts.h"

/*         
*   @className KeyBoardDriver
*   @brief     处理键盘输入 
*/
class KeyBoardDriver : public InterruptHandler
{
public:
    KeyBoardDriver(InterruptManager* manager);
    ~KeyBoardDriver();

    /*           
    *	@brief          键盘输入中断
    *	@param[in]      esp              堆栈栈顶
    *	@return    
    */
    virtual uint32_t HandleInterrupt(uint32_t esp);

private:
    PortOf8Bit m_oDataPort;                //数据端口
    PortOf8Bit m_oCommandPort;             //命令端口
};


#endif