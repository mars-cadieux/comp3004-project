#include "devicelight.h"
#include "neureset.h"

DeviceLight::DeviceLight(QObject* parent, const QString& t, bool l)
    : QObject{parent}, lit(l)
{
    type = t;
    flashThread = QThread::create([this]{ updateLight(); });
    flashThread->start();

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
    lit = false;
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

        flashThread->sleep(1);
    }
}

void DeviceLight::setLit(bool l)
{
    lit = l;
    emit lightChanged(lit, type);
}

bool DeviceLight::isLit() const
{
    return lit;
}

bool DeviceLight::isFlashing() const
{
    return flashing;
}
