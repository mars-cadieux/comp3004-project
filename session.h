#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr, QDateTime dt = QDateTime::currentDateTime());
    Session(QDateTime dt, float bBefore, float bAfter);

    QDateTime getDateTime() const;
    float getBaselineBefore() const;
    float getBaselineAfter() const;

    void setDateTime(QDateTime dt);
    void setBaselineBefore(float bBefore);
    void setBaselineAfter(float bAfter);

signals:

private:
    QDateTime dateTime;
    float beforeBaseline;
    float afterBaseline;
};

#endif // SESSION_H
