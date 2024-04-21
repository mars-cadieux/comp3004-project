#ifndef DEVICELIGHT_H
#define DEVICELIGHT_H

#include <QThread>

class Neureset;

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

class DeviceLight : public QObject
{
    Q_OBJECT
public:
    explicit DeviceLight(QObject *parent = nullptr, const QString& t = "", bool l = false);
    ~DeviceLight();
    void startFlashing();
    void stopFlashing();
    void updateLight();
    void setLit(bool l);
    bool isLit() const;
    bool isFlashing()const;

signals:
    void lightChanged(bool lit, QString t);

private:
    bool lit;
    bool flashing;
    QThread* flashThread;
    Neureset* neureset; //pointer to the neureset
    QString type;
};

#endif // DEVICELIGHT_H
