#ifndef __MOUSE_H
#define __MOUSE_H

/*         
*   @fileName       mouse.h
*   @brief          处理鼠标中断文件
*   @author         dute  
*/

#include "interrupts.h"
#include "driver.h"
#include "types.h"

class MouseEventHandle
{
public:
    MouseEventHandle();

    /*           
    *	@brief          处理鼠标按下事件,外部可重写该事件
    */
    virtual void OnMouseDown(uint8_t button);

    /*           
    *	@brief          处理鼠标抬起事件,外部可重写该事件
    */
    virtual void OnMouseUp(uint8_t button);

    /*           
    *	@brief          处理鼠标移动事件,外部可重写该事件
    *	@param[in]      xpos    x方向坐标
    *	@param[in]      ypos    y方向坐标
    */
    virtual void OnMouseMove(int8_t xpos, int8_t ypos);

    virtual void OnMouseActivate();

private:
    int8_t m_oXpos;
    int8_t m_oYpos;
};

class MouseDriver : public IDriver , public InterruptHandler
{
public:
    MouseDriver(InterruptManager* manager, MouseEventHandle* pHandler);
    ~MouseDriver();

    /*           
    *	@brief          鼠标输入中断
    *	@param[in]      esp              堆栈栈顶
    *	@return    
    */
    virtual uint32_t HandleInterrupt(uint32_t esp);

    /*           
    *	@brief     激活鼠标驱动程序
    *	@param[in]   
    */
    virtual void Activate();

private:
    PortOf8Bit  m_oDataPort;                //数据端口
    PortOf8Bit  m_oCommandPort;             //命令端口

    uint8_t     m_mouseInfo[3];             //鼠标每次移动都会发送数据包(可能是3Byte,也可能是4Byte的包)
                                            //当前按3Byte包处理
                                            //第1个Byte包含,鼠标坐标信息
                                            //第2个Byte包含,鼠标x方向移动距离
                                            //第3个Byte包含,鼠标y方向移动距离
    
    uint8_t     m_oOffset;                  //因为一次中断只能读取一个字节的内容,表示读取的buffer的偏移值

    uint8_t     m_oButtons;                 //标记鼠标左右按键此时的状态(按下或者释放)

    //因为uint8_t不能存负值,所以当m_oXpos < 0时,又会变成255;相当于,鼠标放在最左侧时,鼠标会从最右面穿过来
    //所以应该用int8_t
    int8_t     m_oXpos;                    //鼠标的x坐标,
    int8_t     m_oYpos;                    //鼠标的y坐标

    MouseEventHandle* m_pHandle;
};

#endif