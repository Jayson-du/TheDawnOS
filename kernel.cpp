#include "types.h"
#include "gdt.h"

//最原始的操作系统是没有标准库的printf,因此需要自定义一个printf
void printf(char* str)
{
    //注意!0xb8000内存地址是显示器地址,往这里写数据可以直接输出到屏幕上
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for(int i = 0;i < str[i];i++)
    {
        //取或0xff00的意思是我们需要把屏幕高四位拉低，否则就是黑色的字体，黑色的字体黑色的屏幕是啥也看不到的
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

typedef void(*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;


/*           
*@brief         获取.init_array段的地址并执行段中的指令,保证初始化存放于.init_array段中必要的全局变量
*/
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();         //此时无法确定start_ctors与end_ctors的具体地址,需要告诉链接器由它来确定具体地址,
                        //然后将初始化指令放在起始地址和终止地址之间,保证在循环中两者之间的指令都能被执行
    }
}

/*           
*@brief         操作系统主启动函数    
*@param[in]     multiboot_strcuture
*@param[in]     magicnumbers
*/
extern "C" void kernelMain(void* multiboot_strcuture, uint32_t magicnumber)
{
    printf((char*)"Hello World!");

    GlobalDescriptorTable gdt;
    while(1);
}