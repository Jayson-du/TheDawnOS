#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/*         
*   @fileName	keyboard.h
*   @brief    	键盘驱动
*   @author	    dute
*/

#include "interrupts.h"
#include "driver.h"

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
class KeyBoardDriver : public IDriver, public InterruptHandler
{
public:
    KeyBoardDriver(InterruptManager* manager, KeyBoardEventHandler* handler);
    ~KeyBoardDriver();

    /*           
    *	@brief          键盘输入中断
    *	@param[in]      esp              堆栈栈顶
    *	@return    
    */
    virtual uint32_t HandleInterrupt(uint32_t esp);

    /*           
    *	@brief     激活键盘驱动
    *	@param[in]   
    */
    virtual void Activate();

private:
    PortOf8Bit m_oDataPort;                //数据端口
    PortOf8Bit m_oCommandPort;             //命令端口

    KeyBoardEventHandler* m_pHandler;      //键盘事件
};

#endif