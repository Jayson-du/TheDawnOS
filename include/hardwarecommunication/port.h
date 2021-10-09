#ifndef __PORT_H
#define __PORT_H

/*         
*   fileName    :   Port.h
*   @brief      :   I/O操作文件
*/
#include "types.h"

/*         
*   className   :   Port
*   @brief      :   读写端口基类
*/
class Port
{
protected:
    Port(uint16_t Portnumber);
    ~Port();

protected:
    uint16_t m_nPortNumber;
};

/*         
*   @className  :   PortOf8Bit
*   @brief      :   读1个字节的内容
*/
class PortOf8Bit : public Port
{
public:
    PortOf8Bit(uint16_t Portnumber);
    ~PortOf8Bit();

    virtual void Write(uint8_t data);
    virtual uint8_t Read();
};

/*         
*   className   :   PortOf8BitSlow
*   @brief      :   慢写1个字节
*/
class PortOf8BitSlow : public PortOf8Bit
{
public:
    PortOf8BitSlow(uint16_t Portnumber);
    ~PortOf8BitSlow();

    virtual void Write(uint8_t data);
};

/*         
*   className   :   PortOf16Btye
*   @brief      :   读2个字节的内容
*/
class PortOf16Bit : public Port
{
public:
    PortOf16Bit(uint16_t Portnumber);
    ~PortOf16Bit();


    virtual void Write(uint16_t data);
    virtual uint16_t Read();
};

/*         
*   className   :   PortOf32Btye
*   @brief      :   读4个字节的内容
*/
class PortOf32Bit : public Port
{
public:
    PortOf32Bit(uint16_t Portnumber);
    ~PortOf32Bit();

    virtual void Write(uint32_t data);
    virtual uint32_t Read();
};

#endif