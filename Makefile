#编译参数
#			-m32							指定编译为32位应用程序
#			-fno-use-cxa-atexit				禁用C析构函数__cxa_atexit
#			-fleading-underscore			编译时在C语言符号前加上下划线
#			-fno-exceptions					禁用异常机制
#			-fno-builtin					不使用C语言的内建函数
#			-nostdlib						不链接系统标准启动文件和标准库文件
#			-fno-rtti						禁用运行时类型信息
#			-fno-pie						禁用PIE模式

GPPPARAMS = -m32							\
			-fno-use-cxa-atexit				\
			-fleading-underscore			\
			-fno-exceptions					\
			-fno-builtin					\
			-nostdlib						\
			-fno-rtti						\
			-fno-pie						\

ASPARAMS = --32
LDPARAMS = -melf_i386 						\
		   -no-pie

objects = loader.o 							\
          kernel.o							\
		  gdt.o

%.o: %.cpp
	g++ ${GPPPARAMS} -o $@ -c $<

%.o: %.s
	as ${ASPARAMS} -o $@ $<

mykernel.bin: linker.ld ${objects}
	ld ${LDPARAMS} -T $< -o $@ ${objects}

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "my os" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: mykernel.iso						#make run,启动虚拟机挂在mykernel.iso镜像
	virtualbox --startvm "TheDawnOS"


