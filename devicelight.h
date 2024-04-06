#ifndef DEVICELIGHT_H
#define DEVICELIGHT_H


class DeviceLight
{
public:
    DeviceLight();
    void startFlashing();
    void stopFlashing();
    void updateLight();
    bool isLit();
private:
    bool lit;
    bool flashing;
};

#endif // DEVICELIGHT_H
