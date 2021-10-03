#ifndef __PORT_H
#define __PORT_H

/*         
*   fileName    :   port.h
*   @brief      :   I/O操作文件
*/

#include "types.h"

/*         
*   className   :   port
*   @brief      :   读写端口基类
*/
class port
{
protected:
    port(uint16_t portnumber);
    ~port();

protected:
    uint16_t m_nPortNumber;
};

/*         
*   @className  :   portOf8Bit
*   @brief      :   读1个字节的内容
*/
class portOf8Bit : public port
{
public:
    portOf8Bit(uint16_t portnumber);
    ~portOf8Bit();

protected:
    virtual void write(uint8_t data);
    virtual uint8_t read();
};

/*         
*   className   :   portOf8BitSlow
*   @brief      :   慢写1个字节
*/
class portOf8BitSlow : public portOf8Bit
{
public:
    portOf8BitSlow(uint16_t portnumber);
    ~portOf8BitSlow();

    virtual void write(uint8_t data);
};

/*         
*   className   :   portOf16Btye
*   @brief      :   读2个字节的内容
*/
class portOf16Bit : public port
{
public:
    portOf16Bit(uint16_t portnumber);
    ~portOf16Bit();


    virtual void write(uint16_t data);
    virtual uint16_t read();
};

/*         
*   className   :   portOf32Btye
*   @brief      :   读4个字节的内容
*/
class portOf32Bit : public port
{
public:
    portOf32Bit(uint16_t portnumber);
    ~portOf32Bit();

    virtual void write(uint32_t data);
    virtual uint32_t read();
};

#endif