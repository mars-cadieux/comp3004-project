#include "devicelight.h"

DeviceLight::DeviceLight()
{
    flashThread = QThread::create([this]{ updateLight(); });
    flashThread->start();

    lit = true;
    flashing = false;
}

void DeviceLight::startFlashing()
{
    flashing = true;
}
void DeviceLight::stopFlashing()
{
    flashing = false;
}

void DeviceLight::updateLight()
{

    while(flashThread->isRunning())
    {
        if(flashing)
        {
            lit = !lit;
        }
        else
        {
            lit = true;
        }

        flashThread->sleep(1);
    }
}

bool DeviceLight::isLit()
{
    return lit;
}
