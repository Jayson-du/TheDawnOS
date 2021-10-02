#现代操作系统使用UEFI启动,因此Boot程序Mutileboot规范来编译内核,
#按照Mutileboot规范,内核必须在起始的8KB中的(512字节)包含一个多引导项头(Multiboot header),
#多引导项头里必须有3个4字节对齐块

.set MAGIC, 0x1badb002                    #GRUB魔术块,包含了魔数[0x1BADBOO2],是多引导项头结构的定义值
.set FLAGS, (1 << 0 | 1 << 1)             #GRUB标志块,简单设置为0
.set CHECKSUM, -(MAGIC + FLAGS)           #GRUB校验块,魔数块与校验块的数值总和必须为0

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text
.extern _kernelMain                       #extern,kernelMain为外部函数,调用的时候可以遍历所有文件找到该函数并使用
                                          #导入kernelMain函数,加下划线是因为kernelMain函数是按extern "C"编译的
.extern _callConstructors                 #导入系统构造函数
.global loader                            #global让符号对链接器可见,可以供其他链接对象模块使用,让loader可见,
                                          #这样链接器就知道跳转到什么地方开始并执行;

loader:
  mov $kernel_stack, %esp

  call _callConstructors                  #保证初始化存放于.init_array段中必要的全局变量,需要调用_callConstructors
  push %eax
  push %ebx
  call _kernelMain

_stop:
  cli                                     #将IF置0,屏蔽掉"可屏蔽中断",当屏蔽中断到来时,CPU不会响应,继续执行原有指令
  hlt                                     #处理器暂停处理,禁用中断后使用hlt暂停CPU，以后无法再唤醒
  jmp _stop

.section .bss                             #BSS(block Started by Symbol)段通常指用来存放程序中未初始化的全局变量的一块内存区域;
                                          #BSS属于静态内存分配;
.space 2*1024*1024                        #.space为.bss段分配2M大小的内存空间
kernel_stack: