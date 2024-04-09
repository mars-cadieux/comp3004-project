#ifndef DEVICELIGHT_H
#define DEVICELIGHT_H

#import <QThread>

class DeviceLight
{
public:
    DeviceLight();
    void startFlashing();
    void stopFlashing();
    void updateLight();
    bool isLit();
    bool isFlashing();

private:
    bool lit;
    bool flashing;
    QThread* flashThread;
};

#endif // DEVICELIGHT_H
