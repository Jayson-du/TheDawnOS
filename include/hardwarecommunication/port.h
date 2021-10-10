#ifndef __TDOS__PORT_H
#define __TDOS__PORT_H

/*         
*   fileName    :   Port.h
*   @brief      :   I/O操作文件
*/
#include "common/types.h"

namespace TDOS
{
    namespace HardWareCommunication
    {
        /*         
        *   className   :   Port
        *   @brief      :   读写端口基类
        */
        class Port
        {
        protected:
            Port(Common::uint16_t Portnumber);
            ~Port();

        protected:
            TDOS::Common::uint16_t m_nPortNumber;
        };

        /*         
        *   @className  :   PortOf8Bit
        *   @brief      :   读1个字节的内容
        */
        class PortOf8Bit : public Port
        {
        public:
            PortOf8Bit(Common::uint16_t Portnumber);
            ~PortOf8Bit();

            virtual void Write(Common::uint8_t data);
            virtual Common::uint8_t Read();
        protected:

           /*
            *   brief   用内联函数能提高效率
            *   inb (Bit, Port):   从I/O端口读取一个字节
            *   outb(Bit, Port):   向I/O端口写入一个字节
            */
            static inline void Write8Bit(Common::uint8_t _ndata, Common::uint16_t _nport)
            {
                /*从data读取一个字节写入m_nPortNumber*/
                __asm__ volatile("outb %0, %1" : : "a" (_ndata), "Nd" (_nport));
            }

            static inline Common::uint8_t Read8Bit(Common::uint16_t _nport)
            {
                Common::uint8_t result;
                __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd"(_nport));
                return result;
            }
        };

        /*         
        *   className   :   PortOf8BitSlow
        *   @brief      :   慢写1个字节
        */
        class PortOf8BitSlow : public PortOf8Bit
        {
        public:
            PortOf8BitSlow(Common::uint16_t Portnumber);
            ~PortOf8BitSlow();

            virtual void Write(Common::uint8_t data);

        protected:
            static inline void Write8BitSlow(Common::uint8_t _ndata, Common::uint16_t _nport)
            {
                __asm__ volatile("outb %0, %1\njmP 1f\n1: jmP 1f\n1:" : : "a" (_ndata), "Nd" (_nport));
            }
        };

        /*         
        *   className   :   PortOf16Btye
        *   @brief      :   读2个字节的内容
        */
        class PortOf16Bit : public Port
        {
        public:
            PortOf16Bit(Common::uint16_t Portnumber);
            ~PortOf16Bit();


            virtual void Write(Common::uint16_t data);
            virtual Common::uint16_t Read();

        protected:

            /*
            *   brief   用内联函数能提高效率
            *   inl (Word, Port):   从I/O端口读取32位数据
            *   outl(Word, Port):   向I/O端口写入32位数据
            */
            static inline void Write16Bit(Common::uint16_t _ndata, Common::uint16_t _nport)
            {
                /*从data读取一个字写入m_nPortNumber*/
                __asm__ volatile("outw %0, %1" : : "a" (_ndata), "Nd" (_nport));
            }

            static inline Common::uint16_t Read16Bit(Common::uint16_t _nport)
            {
                Common::uint16_t result;
                __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd"(_nport));
                return result;
            }
        };

        /*         
        *   className   :   PortOf32Btye
        *   @brief      :   读4个字节的内容
        */
        class PortOf32Bit : public Port
        {
        public:
            PortOf32Bit(Common::uint16_t Portnumber);
            ~PortOf32Bit();

            virtual void Write(Common::uint32_t data);
            virtual Common::uint32_t Read();
        protected:
            /*  brief   用内联函数可以提升效率
            *   inl (Word, Port):   从I/O端口读取32位数据
            *   outl(Word, Port):   向I/O端口写入32位数据
            */
            static inline void Write32Bit(Common::uint32_t _ndata, Common::uint16_t _nport)
            {
                /*从data读取一个32位数据写入m_nPortNumber*/
                __asm__ volatile("outl %0, %1" : : "a" (_ndata), "Nd" (_nport));
            }

            static inline Common::uint32_t Read32Bit(Common::uint16_t _nport)
            {
                Common::uint32_t result;
                __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd"(_nport));
                return result;
            }
        };
    }
}

#endif