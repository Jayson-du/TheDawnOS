#include "drivers/driver.h"

using namespace TDOS::Common;
using namespace TDOS::Drivers;

void IDriver::Activate() 
{
    
}

int IDriver::Reset() 
{
    return 0;
}

void IDriver::Deactive() 
{

}

DriverManager::DriverManager() 
    : m_nDrivers(0)
{
    
}

void DriverManager::ActivateAllHardWare() 
{
    for(uint16_t i = 0;i < 256; i++)
    {
        if(NULL != m_DriverArray[i])
        {
            m_DriverArray[i]->Activate();
        }
    }
}

void DriverManager::AddDriver(IDriver* pDriver) 
{
    m_DriverArray[m_nDrivers++] = pDriver;
}


