#include "keyboard.h"

void printf(const char* str);

/*
*   brief
*   键盘中有一颗键盘编码器芯片,通常是Intel8048及兼容芯片,它的作用是监视键盘并把相应的数据传给PC;在PC机的主板上
*有一个键盘控制器,通常是Intel8042及兼容芯片,它的作用是用来接收和解码来看键盘的数据并放到缓冲区中,然后通知8259A产生
*中断IRQ1;
*   读缓冲区:           读0x60端口
*   写缓冲区:           写0x60端口
*   读状态寄存器:       读0x64端口
*   控制寄存器:         写0x64端口
*/
KeyBoardDriver::KeyBoardDriver(InterruptManager* manager)
    //0x01 +  HardWareInterruptOffset = 0x21即键盘中断IRQ1
    : InterruptHandler(0x01 + manager->GetHardWareInterruptOffset(), manager)
    , m_oDataPort(0x60)
    , m_oCommandPort(0x64)
{
    while(m_oCommandPort.Read() & 0x1)         //CommandPort第1位为1时,表示当前输出缓冲区是满的
    {
        m_oDataPort.Read();                     //当从DataPort读取数据后,CommandPort第1位清零
    }
    m_oCommandPort.Write(0xae);                 //0xae通知键盘,已经写完了,激活键盘
    
    m_oCommandPort.Write(0x20);                 //准备读取8042芯片的Command Byte,
                                                //其行为是将当前的8042 Command Byte的内容放置于Output Register中,
                                                //下一个从0x60端口读操作将会将其取出来

    uint8_t status = (m_oDataPort.Read() | 1) & ~0x10 ;  //读取,把第4为置0,开启键盘及键盘中断

    m_oCommandPort.Write(0x60);                 //通知键盘,这个操作是一个写入操作
    m_oDataPort.Write(status);
    m_oDataPort.Write(0xf4);                    //通知键盘上的8048芯片,写操作已完成;
                                                //8048芯片收到后,将会清空输出缓冲区,回复一个ACK,然后继续接收keyboard的击键
}

KeyBoardDriver::~KeyBoardDriver() 
{
    
}

uint32_t KeyBoardDriver::HandleInterrupt(uint32_t esp) 
{
    uint8_t key = m_oDataPort.Read();
    char* foo = "KEYBOARD 0X00!";
    const char* hex = "0123456789ABCDEF";
    foo[22] = hex[(key >> 4) & 0x0f];
    foo[23] = hex[(key & 0x0f)];
    printf(foo); 
    return esp;
}
