#include "session.h"

Session::Session(QObject *parent, QDateTime dt)
    : QObject{parent}, dateTime(dt)
{
    beforeBaseline = 0.0;
    afterBaseline = 0.0;
}

//custom copy ctor used when making a deep copy of a session which gets uploaded to the PC. Note that this ctor always sets uploaded to true; this ctor should only be used for uploading
Session::Session(const Session &s): dateTime(s.dateTime), beforeBaseline(s.beforeBaseline), afterBaseline(s.afterBaseline)
{
    uploaded = true;
}

QDateTime Session::getDateTime() const
{
    return dateTime;
}

float Session::getBaselineBefore() const
{
    return beforeBaseline;
}

float Session::getBaselineAfter() const
{
    return afterBaseline;
}

bool Session::isUploaded() const
{
    return uploaded;
}

void Session::setDateTime(QDateTime dt)
{
    dateTime = dt;
}

void Session::setBaselineBefore(float bBefore)
{
    beforeBaseline = bBefore;
}

void Session::setBaselineAfter(float bAfter)
{
    afterBaseline = bAfter;
}

void Session::setuploaded(bool u)
{
    uploaded = u;
}

void Session::print() const
{
    QString dt = dateTime.toString();
    QString before = QString::number(beforeBaseline);
    QString after = QString::number(afterBaseline);
    QString msg = "Session Date: " + dt + "\nBaseline (Before): " + before + "\nBaseline (After): " + after;

    qInfo(qPrintable(msg));
}
