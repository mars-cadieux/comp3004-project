#include "devicelight.h"
#include "neureset.h"

DeviceLight::DeviceLight(QObject* parent, const QString& t)
    : QObject{parent}
{
    type = t;
    flashThread = QThread::create([this]{ updateLight(); });
    flashThread->start();

    lit = true;
    flashing = false;
    emit lightChanged(lit, type);
}

DeviceLight::~DeviceLight()
{

}

void DeviceLight::startFlashing()
{
    flashing = true;
}
void DeviceLight::stopFlashing()
{
    flashing = false;
    lit = true;
    emit lightChanged(lit, type);
}

void DeviceLight::updateLight()
{

    while(flashThread->isRunning())
    {
        if(flashing)
        {
            lit = !lit;
            emit lightChanged(lit, type);
        }
        else
        {
            lit = true;
        }

        flashThread->sleep(2);
    }
}

bool DeviceLight::isLit()
{
    return lit;
}

bool DeviceLight::isFlashing()
{
    return flashing;
}
