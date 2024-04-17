#ifndef DEVICELIGHT_H
#define DEVICELIGHT_H

#include <QThread>

/* DeviceLight
 * Class Purpose: Control the lights on the Neureset UI, allowing them to turn on, off, or flash.
 *
 * Data Members:
 * bool lit: true if light is currently on
 * bool flashing: true if light is currently flashing
 * QThread flashThread: thread to run light asynchronously
 *
 * Class Functions:
 * Getters for lit and flashing status
 * void startFlashing/stopFlashing: Starts and stops the light flashing
 * void updateLight: Run on flashThread, updates light to turn it on and off if flashing.
 * */

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
