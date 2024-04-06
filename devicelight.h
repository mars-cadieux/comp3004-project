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
private:
    bool lit;
    bool flashing;
    QThread* flashThread;
};

#endif // DEVICELIGHT_H
