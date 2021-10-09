#include "keyboard.h"

void printf(const char* str);
void printfHex(uint8_t data);

KeyBoardEventHandler::KeyBoardEventHandler() 
{
    
}

void KeyBoardEventHandler::OnKeyDown(char key) 
{
    char* ch = (char*)" ";
    ch[0] = key;
    printf(ch);
}

void KeyBoardEventHandler::OnKeyUp(char key) 
{
    
}

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
KeyBoardDriver::KeyBoardDriver(InterruptManager* manager, KeyBoardEventHandler* handler)
    //0x01 +  HardWareInterruptOffset = 0x21即键盘中断IRQ1
    : InterruptHandler(0x01 + manager->GetHardWareInterruptOffset(), manager)
    , m_oDataPort(0x60)
    , m_oCommandPort(0x64)
    , m_pHandler(handler)
{

}

KeyBoardDriver::~KeyBoardDriver() 
{
    
}

uint32_t KeyBoardDriver::HandleInterrupt(uint32_t esp) 
{
    uint8_t key = m_oDataPort.Read();
    
    if(NULL == m_pHandler)
        return esp;

    static bool shift = false;

    switch (key) 
    {
    case 0x02: if (shift) m_pHandler->OnKeyDown('!'); else m_pHandler->OnKeyDown('1'); break;
    case 0x03: if (shift) m_pHandler->OnKeyDown('@'); else m_pHandler->OnKeyDown('2'); break;
    case 0x04: if (shift) m_pHandler->OnKeyDown('#'); else m_pHandler->OnKeyDown('3'); break;
    case 0x05: if (shift) m_pHandler->OnKeyDown('$'); else m_pHandler->OnKeyDown('4'); break;
    case 0x06: if (shift) m_pHandler->OnKeyDown('%'); else m_pHandler->OnKeyDown('5'); break;
    case 0x07: if (shift) m_pHandler->OnKeyDown('^'); else m_pHandler->OnKeyDown('6'); break;
    case 0x08: if (shift) m_pHandler->OnKeyDown('&'); else m_pHandler->OnKeyDown('7'); break;
    case 0x09: if (shift) m_pHandler->OnKeyDown('*'); else m_pHandler->OnKeyDown('8'); break;
    case 0x0A: if (shift) m_pHandler->OnKeyDown('('); else m_pHandler->OnKeyDown('9'); break;
    case 0x0B: if (shift) m_pHandler->OnKeyDown(')'); else m_pHandler->OnKeyDown('0'); break;

    case 0x10: if (shift) m_pHandler->OnKeyDown('Q'); else m_pHandler->OnKeyDown('q'); break;
    case 0x11: if (shift) m_pHandler->OnKeyDown('W'); else m_pHandler->OnKeyDown('w'); break;
    case 0x12: if (shift) m_pHandler->OnKeyDown('E'); else m_pHandler->OnKeyDown('e'); break;
    case 0x13: if (shift) m_pHandler->OnKeyDown('R'); else m_pHandler->OnKeyDown('r'); break;
    case 0x14: if (shift) m_pHandler->OnKeyDown('T'); else m_pHandler->OnKeyDown('t'); break;
    case 0x15: if (shift) m_pHandler->OnKeyDown('Z'); else m_pHandler->OnKeyDown('z'); break;
    case 0x16: if (shift) m_pHandler->OnKeyDown('U'); else m_pHandler->OnKeyDown('u'); break;
    case 0x17: if (shift) m_pHandler->OnKeyDown('I'); else m_pHandler->OnKeyDown('i'); break;
    case 0x18: if (shift) m_pHandler->OnKeyDown('O'); else m_pHandler->OnKeyDown('o'); break;
    case 0x19: if (shift) m_pHandler->OnKeyDown('P'); else m_pHandler->OnKeyDown('p'); break;

    case 0x1E: if (shift) m_pHandler->OnKeyDown('A'); else m_pHandler->OnKeyDown('a'); break;
    case 0x1F: if (shift) m_pHandler->OnKeyDown('S'); else m_pHandler->OnKeyDown('s'); break;
    case 0x20: if (shift) m_pHandler->OnKeyDown('D'); else m_pHandler->OnKeyDown('d'); break;
    case 0x21: if (shift) m_pHandler->OnKeyDown('F'); else m_pHandler->OnKeyDown('f'); break;
    case 0x22: if (shift) m_pHandler->OnKeyDown('G'); else m_pHandler->OnKeyDown('g'); break;
    case 0x23: if (shift) m_pHandler->OnKeyDown('H'); else m_pHandler->OnKeyDown('h'); break;
    case 0x24: if (shift) m_pHandler->OnKeyDown('J'); else m_pHandler->OnKeyDown('j'); break;
    case 0x25: if (shift) m_pHandler->OnKeyDown('K'); else m_pHandler->OnKeyDown('k'); break;
    case 0x26: if (shift) m_pHandler->OnKeyDown('L'); else m_pHandler->OnKeyDown('l'); break;

    case 0x2C: if (shift) m_pHandler->OnKeyDown('Y'); else m_pHandler->OnKeyDown('y'); break;
    case 0x2D: if (shift) m_pHandler->OnKeyDown('X'); else m_pHandler->OnKeyDown('x'); break;
    case 0x2E: if (shift) m_pHandler->OnKeyDown('C'); else m_pHandler->OnKeyDown('c'); break;
    case 0x2F: if (shift) m_pHandler->OnKeyDown('V'); else m_pHandler->OnKeyDown('v'); break;
    case 0x30: if (shift) m_pHandler->OnKeyDown('B'); else m_pHandler->OnKeyDown('b'); break;
    case 0x31: if (shift) m_pHandler->OnKeyDown('N'); else m_pHandler->OnKeyDown('n'); break;
    case 0x32: if (shift) m_pHandler->OnKeyDown('M'); else m_pHandler->OnKeyDown('m'); break;
    case 0x33: if (shift) m_pHandler->OnKeyDown('<'); else m_pHandler->OnKeyDown(','); break;
    case 0x34: if (shift) m_pHandler->OnKeyDown('>'); else m_pHandler->OnKeyDown('.'); break;
    case 0x35: if (shift) m_pHandler->OnKeyDown('_'); else m_pHandler->OnKeyDown('-'); break;

    case 0x1C: m_pHandler->OnKeyDown('\n'); break;
    case 0x39: m_pHandler->OnKeyDown(' '); break;
    case 0x2A: case 0x36: shift = true; break;
    case 0xAA: case 0xB6: shift = false; break;

    case 0x45: break; // NumLock

    default:
        if (key < 0x80) {
            printf("KEYBOARD 0x");
            printfHex(key);
        }
    }
    
    return esp;
}

void KeyBoardDriver::Activate() 
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

