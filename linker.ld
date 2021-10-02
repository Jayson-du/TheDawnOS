/*
*   @brief     linker脚本文件,用来控制link过程的文件,文件中包含内容为linker的处理命令,
               主要用于描述输入文件到输出文件时,各个内容的分布及内存映射等;
               具体语法详见:https://blog.csdn.net/m0_47799526/article/details/108765403
*/

ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS 
{
    . = 0x0100000;

    .text :                             /*告诉链接器,如何将输入部分映射到输出部分,以及如何将输出部分放在内存中*/
    {
        *(.multiboot)
        *(.text*)                       
        *(.rodata)
    }

    .data :                             /*建立链接脚本文件linker.ld使用下面的指令保证执行.init_array段的部分*/
    {
        _start_ctors = .;               /*告诉链接器把需要初始化的部分放在_start_ctors和_end_ctors之间*/
        /* 
        *  在init_array中构造的全局变量,使用KEEP禁止释放资源
        */
        KEEP(*( .init_array ));         /*GCC使用SORT_BY_INIT_PRIORITY将section按升序排序,并放到输出文件中*/
        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
        _end_ctors = .;

        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.fini_array*)
        *(.comment)
    }
}