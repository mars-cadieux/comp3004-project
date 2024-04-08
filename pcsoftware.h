#ifndef PCSOFTWARE_H
#define PCSOFTWARE_H

#include <QObject>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

#include "pcui.h"

class Session;

class PCSoftware : public QObject
{
    Q_OBJECT
public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

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
    QSqlDatabase sessionDB;
    bool DBInit();
    PCui* pcui;

};

#endif // PCSOFTWARE_H
