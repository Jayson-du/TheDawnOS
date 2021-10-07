#因为中断放在CPU的中断寄存器中,因此需要汇编语言来操作寄存器
.set IRQ_BASE, 0x20                                   #处理中断的函数HandleInterruptRequest的基址
.section .text                                        #代码段
.extern __ZN16InterruptManager15HandleInterruptEhj    #声明interrupts.h中的InterruptsManager::handleInterrupt函数

#汇编语言的宏定义
.macro handleInterruptRequest num                                   #中断宏定义
.global __ZN16InterruptManager26HandleInterruptRequest\num\()Ev     #()是为了区分num与Ev
__ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleException num                                          #异常宏定义
.global __ZN16InterruptManager19HandleException\num\()Ev
__ZN16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


HandleInterruptRequest 0x00

HandleInterruptRequest 0x01

HandleInterruptRequest 0x02

HandleInterruptRequest 0x03

HandleInterruptRequest 0x04

HandleInterruptRequest 0x05

HandleInterruptRequest 0x06

HandleInterruptRequest 0x07

HandleInterruptRequest 0x08

HandleInterruptRequest 0x09

HandleInterruptRequest 0x0A

HandleInterruptRequest 0x0B

HandleInterruptRequest 0x0C

HandleInterruptRequest 0x0D

HandleInterruptRequest 0x0E

HandleInterruptRequest 0x0F

HandleInterruptRequest 0x31


HandleException 0x00

HandleException 0x01

HandleException 0x02

HandleException 0x03

HandleException 0x04

HandleException 0x05

HandleException 0x06

HandleException 0x07

HandleException 0x08

HandleException 0x09

HandleException 0x0A

HandleException 0x0B

HandleException 0x0C

HandleException 0x0D

HandleException 0x0E

HandleException 0x0F

HandleException 0x10

HandleException 0x11

HandleException 0x12

HandleException 0x13

int_bottom:
    #pusha:把通用寄存器压栈(寄存器的入站顺序一次是eax,ecx,edx,ebx,esp,ebp,esi,edi),pushl压入4byte
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call __ZN16InterruptManager15HandleInterruptEhj

    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

.global __ZN16InterruptManager15InterruptIgnoreEv
__ZN16InterruptManager15InterruptIgnoreEv:

    iret                                                #中断

.data
    interruptnumber: .byte 0
