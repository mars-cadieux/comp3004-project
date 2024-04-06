#include "devicelight.h"

DeviceLight::DeviceLight()
{

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
    // Do nothing for now (Will be used to update the light so it goes from lit to unlit when flashing)
}

bool DeviceLight::isLit()
{
    return lit;
}
