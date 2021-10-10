#ifndef __TDOS__INTERRUPTS_H
#define __TDOS__INTERRUPTS_H

/*         
*   @fileName	interrupts.h
*   @brief      操作中断
*   @author     durz
*/

/*         
*   @brief 
*   Early PC using the Intel 8086/8088 process only had a single PC, and are therefore Iimited
* to 8 interrupts; This was expected to 2 PICs with the introduction of the 286 based PCs;
*
*   Typically on system using the Intel 8259 PIC, 16 IRQs are used. IRQs 0 to 7 are managed by
* 1 Intel 8259, and IRQs 8 to 15 by a second Intel 8259 PIC; The first PIC, the master, is the only
* one that directly signals the CPU. The second PIC, the slave, instead signals to the master on its
* IRQ 2 line, and the master passes the signal on to the CPU; There are therefore only 15 interrupt 
* request linews available for hardware;
*
*   On APIC with IOAPIC system, typically there are 24 IRQs available, and the extra 8 IRQs are 
* used to route PCI interrupts, avoiding conflict between dynamically configured PCI interrupts
* and statically configured ISA interrupts; On early APIC system with only 16 IRQs or with only
* Intel 8259 interrupt controllers, PCI interrupt lines were routed to the 16 IRQs using a 
* PIR(PCI interrupt routing) integrated into the southbridge. On APIC with MSI  system, typically
* there are 224 interrupts available;
*
*   The easiest way of viewing this information on Windows is to use Device Manager or System Information
*(msinfo32.exe). On Linux, IRQ mapping can be viewed by executing "cat /proc/interrupts" or 
*using the procinfo Utility
*
*Master PIC
*   @IRQ 0 - system timer(cannot be changed)            ---- 0x20
*   @IRQ 1 - keyboard controller(cannot be changed)     ---- 0x21
*   @IRQ 2 - cascaded signals from IRQs 8-15(any devices configured to use IRQ 2 will actually be using IRQ9)
*   @IRQ 3 - serial port controller for serial port2(shared with serial port4, if present)
*   @IRQ 4 - serial port controller for serial port1(shared with serial port3, if present)
*   @IRQ 5 - parallel por2 and 3 or sound card
*   @IRQ 6 - floppy disk controller
*   @IRQ 7 - parallel port 1. It is used for printers or for any parallel port if a printer is not
*present. It can also be potentially be shared with a secondary sound card with careful management 
*of the port
*
*Slave PIC
*   @IRQ 8 - real-time clock(RTC)
*   @IRQ 9 - Advanced Configuration and Power Interface(ACPI) system control interrupt on Intel Chipsets.
*Other chipest manufactures might use another interrupt for this purpose, or make it available for
*the use of peripherals( any devices configured to use IRQ2 will actually be using IRQ9)
*   @IRQ 10 - The interrupt is left open for the use of perihperals(open interrupt/available, SCSI or NIC)
*   @IRQ 11 - The interrupt is left open for the use of perihperals(open interrupt/available, SCSI or NIC)
*   @IRQ 12 - mouse on PS/2 connector
*   @IRQ 13 - CPU co-processor or integrated floating point unit or inter-porcessor interrupt(use depends on OS)
*   @IRQ 14 - primary ATA channel (ATA interface usually serves hard disk drivers and CD drivers)
*   @IRQ 15 - secondary ATA channel
*/

#include "common/types.h"
#include "port.h"
#include "../gdt.h"

namespace TDOS
{
    namespace HardWareCommunication
    {
        /*
        *   @className  InterruptHandler
        *   @brief      处理中断基类
        */
        class InterruptHandler;

        /*
        *   @brief      
        */
        struct GateDescriptor
        {
            Common::uint16_t handlerAddressLowBits;
            Common::uint16_t gdt_codeSegmentSelector;
            Common::uint8_t  reserved;
            Common::uint8_t  access;
            Common::uint16_t handlerAddressHighBits;
        }__attribute__((packed));                                   //让编译器不进行内存对齐操作

        struct InterruptDescriptorTablePointer
        {
            Common::uint16_t size;
            Common::uint32_t base;
        }__attribute__((packed));


        /*         
        *   @className  InterruptManager
        *   @brief      中断管理器
        */
        class InterruptManager
        {
            friend class InterruptHandler;
        public:
            InterruptManager(Common::uint16_t nHardWareInterruptOffset, GlobalDescriptorTable* gdt);
            ~InterruptManager();

            /*           
            *	@brief     处理中断函数
            *	@param[in] interruptNumber  中断号
            *	@param[in] esp              堆栈栈顶
            *   @return    处理玩中断后,返回堆栈栈顶
            */
            static Common::uint32_t HandleInterrupt(Common::uint8_t interruptNumber, Common::uint32_t esp);

            Common::uint32_t DoHandleInterrupt(Common::uint8_t interruptNumber, Common::uint32_t esp);

            /*           
            *	@brief     CPU开启中断
            */
            void Activate();
            
            /*           
            *	@brief     CPU关闭中断
            */
            void Deactivate();

            Common::uint16_t GetHardWareInterruptOffset()
            {
                return m_nHardWareInterruptOffset;
            }

        protected:
            static GateDescriptor interruptDescriptorTable[256];

            static InterruptManager* spActiveInterruptManager;

            static void SetInterruptDescriptorTableEntry
            (
                Common::uint8_t     interruptNumber,
                Common::uint16_t    codeSegmentSelectorOffset,
                void                (*handler)(),
                Common::uint8_t     DescriptorPrivilegelLevel,
                Common::uint8_t     DescriptorType
            );

            static void InterruptIgnore();

            Common::uint16_t m_nHardWareInterruptOffset;

            InterruptHandler* m_handlerarray[256];

        protected:
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

        protected:
        /*====================异常====================*/
            static void HandleException0x00();

            static void HandleException0x01();

            static void HandleException0x02();

            static void HandleException0x03();

            static void HandleException0x04();

            static void HandleException0x05();

            static void HandleException0x06();

            static void HandleException0x07();

            static void HandleException0x08();

            static void HandleException0x09();

            static void HandleException0x0A();

            static void HandleException0x0B();

            static void HandleException0x0C();

            static void HandleException0x0D();

            static void HandleException0x0E();

            static void HandleException0x0F();

            static void HandleException0x10();

            static void HandleException0x11();

            static void HandleException0x12();

            static void HandleException0x13();

        protected:
            /*           
            *	@brief     处理异常
            */
            void DealWithException(Common::uint16_t& codeSegment, const Common::uint8_t& IDT_INTERRUPT_GATE);

            /*           
            *	@brief     处理中断
            */
            void DealWithInterrupts(Common::uint16_t& codeSegment, const Common::uint8_t& IDT_INTERRUPT_GATE);

        private:
            /*
            *8259A中断芯片通过两个I/O地址来进行中断相关的数据传输,告诉CPU发生了中断
            *通常情况下是单个使用或者两级级连(个人理解是两个并联)
            *使用单个8259A芯片,传输的I/O端口是0x20和0x21
            *使用两级级连,传输的I/O端口是0x10和0x11
            *两级级连时,会区分一个主中断与一个从中断
            */
            //主中断
            PortOf8BitSlow m_oPicMasterCommand;
            PortOf8BitSlow m_oPicMasterData;
            //从中断
            PortOf8BitSlow m_oPicSlaveCommand;
            PortOf8BitSlow m_oPicSlaveData;
        };

        class InterruptHandler
        {
        public:
            virtual Common::uint32_t HandleInterrupt(Common::uint32_t esp);

        public:
            InterruptHandler(Common::uint8_t interruptNumber, InterruptManager* pInterruptManager);

            /*
            *	@brief     因为InterruptManager与InterruptHandler相互引用,所以构造函数与析构函数不能为virtual,
            *              否则编译会提示
            *              interrupts.o: In function `__ZN16InterruptHandlerD0Ev':
            *              interrupts.cpp:(.text+0xb30): undefined reference to `__ZdlPvj'
            *              跟编译原理相关,暂时先记一下,后续需要查看编译原理
            */
            //virtual ~InterruptHandler();      //error,编译会报错
            ~InterruptHandler();                //correct,编译正常通过
            
            Common::uint8_t m_nInterruptNumber;
            InterruptManager* m_pInterruptManager;

        };
    }
}

#endif
