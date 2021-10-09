#ifndef __DRIVER_H
#define __DRIVER_H

#include "types.h"

/*         
*   @className  	Driver
*   @brief      
*/
class IDriver
{
public:
    IDriver() = default;
    ~IDriver() = default;

    /*           
    *	@brief     激活驱动程序
    *	@param[in]   
    */
    virtual void Activate();

    /*           
    *	@brief     驱动复位
    */
    virtual int Reset();

    /*           
    *	@brief     关闭驱动程序
    */
    virtual void Deactive();
};

class DriverManager
{
public:
    DriverManager();
    ~DriverManager() = default;

    /*           
    *	@brief     激活所有硬件的驱动程序
    */
    void ActivateAllHardWare();
public:
    void AddDriver(IDriver* pDriver);

private:
    IDriver* m_DriverArray[256];

    int m_nDrivers;
};

#endif