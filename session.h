#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr, QDateTime dt = QDateTime::currentDateTime());
    Session(const Session& s);

    QDateTime getDateTime() const;
    float getBaselineBefore() const;
    float getBaselineAfter() const;
    bool isUploaded() const;

    void setDateTime(QDateTime dt);
    void setBaselineBefore(float bBefore);
    void setBaselineAfter(float bAfter);
    void setuploaded(bool u);

    void print() const;

signals:

private:
    QDateTime dateTime;
    float beforeBaseline;
    float afterBaseline;
    bool uploaded;
};

#endif // SESSION_H
