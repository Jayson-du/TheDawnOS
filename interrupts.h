#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

/*         
*   @fileName	interrupts.h
*   @brief      操作中断
*   @author     durz
*/
#include "types.h"
#include "port.h"

/*         
*   @className  InterruptManager
*   @brief      中断管理器
*/
class InterruptManager
{
public:
    /*           
    *	@brief     处理中断函数
    *	@param[in] interruptNumber  中断号
    *	@param[in] esp              堆栈栈顶
    *   @return    处理玩中断后,返回堆栈栈顶
    */
    static uint32_t HandleInterrupt(uint8_t interruptNumber, uint32_t esp);

/*====================中断====================*/
    /*           
    *	@brief     中断请求号
    */
    static void HandleInterruptRequest0x00();

    static void HandleInterruptRequest0x01();

    static void HandleInterruptRequest0x02();

    static void HandleInterruptRequest0x03();

    static void HandleInterruptRequest0x04();

    static void HandleInterruptRequest0x05();

    static void HandleInterruptRequest0x06();

    static void HandleInterruptRequest0x07();

    static void HandleInterruptRequest0x08();

    static void HandleInterruptRequest0x09();

    static void HandleInterruptRequest0x0A();

    static void HandleInterruptRequest0x0B();

    static void HandleInterruptRequest0x0C();

    static void HandleInterruptRequest0x0D();

    static void HandleInterruptRequest0x0E();

    static void HandleInterruptRequest0x0F();

    static void HandleInterruptRequest0x31();

/*====================异常====================*/
    static void HandleExecption0x00();

    static void HandleExecption0x01();

    static void HandleExecption0x02();

    static void HandleExecption0x03();

    static void HandleExecption0x04();

    static void HandleExecption0x05();

    static void HandleExecption0x06();

    static void HandleExecption0x07();

    static void HandleExecption0x08();

    static void HandleExecption0x09();

    static void HandleExecption0x0A();

    static void HandleExecption0x0B();

    static void HandleExecption0x0C();

    static void HandleExecption0x0D();

    static void HandleExecption0x0E();

    static void HandleExecption0x0F();

    static void HandleExecption0x10();

    static void HandleExecption0x11();

    static void HandleExecption0x12();

    static void HandleExecption0x13();
};

#endif


