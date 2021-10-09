#include "mouse.h"

MouseEventHandle::MouseEventHandle()
    : m_oXpos(40)
    , m_oYpos(12)
{
    
}

void MouseEventHandle::OnMouseDown(uint8_t button) 
{

}

void MouseEventHandle::OnMouseUp(uint8_t button) 
{
    
}

void MouseEventHandle::OnMouseMove(int8_t nXpos, int8_t nYpos) 
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;         //显卡地址
    VideoMemory[m_oYpos * 80 + m_oXpos] = ((VideoMemory[m_oYpos * 80 + m_oXpos] & 0xf000) >> 4 |
                                           (VideoMemory[m_oYpos * 80 + m_oXpos] & 0x0f00) << 4)|
                                           (VideoMemory[m_oYpos * 80 + m_oXpos] & 0x00ff);

    //向右+,向下-
    m_oXpos += nXpos;
    if(m_oXpos < 0)                                    //0为左边极值,小于0,则鼠标处于最左边
        m_oXpos = 0;
    else if(m_oXpos >= 80)                              //80为右边极值,大于80,则鼠标处于最右边
        m_oXpos = 79;
    
    m_oYpos += nYpos;
    if(m_oYpos < 0)                                     //0为上部极值,小于0,则鼠标处于最上部
        m_oYpos = 0;
    else if(m_oYpos >= 25)                              //25为下部极值,大于25,则鼠标处于最下部
        m_oYpos = 24;
    
    //获取完位置后,刷新一下鼠标位置
    VideoMemory[m_oYpos * 80 + m_oXpos] = ((VideoMemory[m_oYpos * 80 + m_oXpos] & 0xf000) >> 4 |
                                           (VideoMemory[m_oYpos * 80 + m_oXpos] & 0x0f00) << 4)|
                                           (VideoMemory[m_oYpos * 80 + m_oXpos] & 0x00ff);


}

void MouseEventHandle::OnMouseActivate() 
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;             //显卡地址
    VideoMemory[m_oYpos * 80 + m_oXpos] = ((VideoMemory[m_oYpos * 80 + m_oXpos] & 0xf000) >> 4 |
                                           (VideoMemory[m_oYpos * 80 + m_oXpos] & 0x0f00) << 4)|
                                           (VideoMemory[m_oYpos * 80 + m_oXpos] & 0x00ff);

}


MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandle* pHandler) 
    : InterruptHandler(0x0C + manager->GetHardWareInterruptOffset(), manager)
    , m_oDataPort(0x60)                                     
    , m_oCommandPort(0x64)                                   //鼠标和键盘都是用8240芯片控制,因此读/写端口一致
    , m_oOffset(0)
    , m_oButtons(0)
    , m_oXpos(40)
    , m_oYpos(12)
    , m_pHandle(pHandler)
{
}

MouseDriver::~MouseDriver() 
{
    
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) 
{
    uint8_t status = m_oCommandPort.Read();                 //因为键盘和鼠标由同一控制器控制

    if(!(status & 0x20) || m_pHandle == NULL)              //因此需要判断是否是鼠标的控制状态
    {
        return esp;                                         //如果不是鼠标,则直接返回
    }

    m_mouseInfo[m_oOffset] = m_oDataPort.Read();            //获取鼠标当前位置
    m_oOffset = (m_oOffset + 1) % 3;                        

    if(0 == m_oOffset)                                      
    {
        m_pHandle->OnMouseMove(m_mouseInfo[1], -m_mouseInfo[2]);

        //判断鼠标按键按下
        for(uint8_t i = 0;i < 3;i++)
        {
            if((m_mouseInfo[0] & (1 << i)) != (m_oButtons & (1 << i)))
            {
                if(m_oButtons & (1 << i))
                    m_pHandle->OnMouseUp(i + 1);
                else
                    m_pHandle->OnMouseDown(i + 1);
            }
        }

        m_oButtons = m_mouseInfo[0];
    }
    return esp;
}

void MouseDriver::Activate() 
{
    if(NULL == m_pHandle)
        return;
    
    m_pHandle->OnMouseActivate();

    m_oCommandPort.Write(0xA8);
    m_oCommandPort.Write(0x20);
    uint8_t status = (m_oDataPort.Read() | 2) & ~0x20;      //启动鼠标中断
    m_oCommandPort.Write(0x60);
    m_oDataPort.Write(status);
    m_oCommandPort.Write(0xD4);                             //要写入鼠标
    m_oDataPort.Write(0xF4);                                //开启扫描

    m_oDataPort.Read();
}


