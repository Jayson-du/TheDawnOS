#include "types.h"
#include "gdt.h"
#include "interrupts.h"

/*
*    显示器方向
*                   xpos
*           ㄧㄧㄧㄧㄧㄧㄧ>
*           |
*           |
*           |
*           |
*     ypos  ∨
*/

void clearScreen(uint8_t& xpos, uint8_t& ypos, uint16_t* VideoMemory, int32_t& position);

/*           
*	@brief     最原始的操作系统是没有标准库的printf,因此需要自定义一个printf
*	@param[in] str要输出的字符串
*/
void printf(const char* str)
{
    //注意!0xb8000内存地址是显示器地址,往这里写数据可以直接输出到屏幕上
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t xpos = 0;                        //每一列的像素点
    static uint8_t ypos = 0;                        //每一行的像素点

    for(int32_t i = 0;str[i];i++)
    {
        switch (str[i])
        {
        case '\n':                                  //如果输入为换行符
            ypos++;                                 //换一行,行数 + 1
            xpos = 0;                               //列数从0开始
            break;
        default:
            //取或0xff00的意思是我们需要把屏幕高四位拉低，否则就是黑色的字体，黑色的字体黑色的屏幕是什么也看不到
            VideoMemory[80 * ypos + xpos] = (VideoMemory[80 * ypos + xpos] & 0xFF00) | str[i];
            xpos++;                                 //下移一位
            break;
        }
        clearScreen(xpos, ypos, VideoMemory, i);
    }
}


typedef void(*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;


/*           
*   @brief      获取.init_array段的地址并执行段中的指令,保证初始化存放于.init_array段中必要的全局变量
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
    printf("Hello World!\n");

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);            //硬件中断偏移是0x20
    interrupts.Activate();                              //激活中断
    while(1);
}

void clearScreen(uint8_t& xpos, uint8_t& ypos, uint16_t* VideoMemory, int32_t& position)
{
    if( xpos >= 80)                               //判断x向是否被填满
    {
        ypos++;                                   //换一行
        xpos = 0;                                 //列数清零
    }
    if(25 <= ypos)                                //判断y向是否被填满
    {
        for(ypos = 0; ypos < 25; ypos++)
        {
            for(xpos = 0; xpos < 80; xpos++)      //将每行的字符清空
            {
                //清空
                VideoMemory[80 * ypos + xpos] = (VideoMemory[80 * ypos + xpos] & 0xFF00) | ' ';
            }
        }

        //全部重置为0
        xpos = 0;
        ypos = 0;
    }
}
