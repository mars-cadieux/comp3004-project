#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);

signals:

private:
    QDateTime dateTime;
    float beforeBaseline;
    float afterBaseline;
};

#endif // SESSION_H
