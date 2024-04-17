#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>

/* Session Class
 * Class Purpose: Contains info about a neurotherapy session
 *
 * Data Members:
 * QDateTime dateTime: holds the date of the session
 * float beforeBaseline: contains baseline frequency before neurotherapy
 * float afterBaseline: contains baseline frequency after neurotherapy
 * bool uploaded: true if uploaded to PC, to prevent double entries
 * float progress: current progress through session as a percentage
 *
 * Class Functions:
 * Getters for dateTime, baselines, uploaded, and progress
 * Setters for dateTime, baselines, and uploaded
 * void updateProgress(): Adds to the session progress variable
 * void print(): Prints a readable string of session info
 * */

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
    float getProgress() const;

    void setDateTime(QDateTime dt);
    void setBaselineBefore(float bBefore);
    void setBaselineAfter(float bAfter);
    void setuploaded(bool u);
    void updateProgress(int prog);

    void print() const;

signals:

private:
    QDateTime dateTime;
    float beforeBaseline;
    float afterBaseline;
    bool uploaded;
    float progress;
};

#endif // SESSION_H
