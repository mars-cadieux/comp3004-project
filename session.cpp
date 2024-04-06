#include "session.h"

Session::Session(QObject *parent, QDateTime dt)
    : QObject{parent}, dateTime(dt)
{

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
