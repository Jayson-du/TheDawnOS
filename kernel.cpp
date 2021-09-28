void printf(char* str)
{
    unsigned short* VideoMemory = (unsigned short*)0xb8000;
    for(int i = 0;i < str[i];i++)
    {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

typedef void(*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_strcuture, unsigned int magicnumber)
{
    printf((char*)"Hello World!");
    while(1);
}