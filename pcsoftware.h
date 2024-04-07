#ifndef PCSOFTWARE_H
#define PCSOFTWARE_H

#include <QObject>
#include <QVector>

#include "pcui.h"

class Session;

class PCSoftware : public QObject
{
    Q_OBJECT
public:
    explicit PCSoftware(QObject *parent = nullptr);
    ~PCSoftware();

    void handleDownloadButton();
    void printSessions() const;

public slots:
    void downloadData(QVector<Session*> sessions);

signals:
    void requestData();
    void sessionDataDownloaded(QVector<Session*> sd);

private:
    QVector<Session*> sessionData;

    PCui* pcui;

};

#endif // PCSOFTWARE_H
