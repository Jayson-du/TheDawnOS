#include "interrupts.h"

void printf(const char* str);

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    printf("interrupts");
    return esp;
}

void InterruptManager::HandleInterruptRequest0x00()
{

}

void InterruptManager::HandleExecption0x00()
{
    
}