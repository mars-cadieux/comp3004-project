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

/* PCSoftware Class
 * Class Purpose: Handles the functions for the PC window, such as managing the session DB and retrieving new session data
 *
 * Data Members:
 * const strings for the date format and database path
 * QVector sessionData: Contains a list of session objects displayed on the PC UI
 * QSqlDatabase sessionDB: Holds the database item, to be updated when new session data is added.
 * PCui* pcui: Contains a pointer to the pcui.
 *
 * Class Functions:
 * void handleDownloadButton(): Sends a signal to neureset requesting to download data
 * void printSessions(): Prints all sessions stored in sessionData
 *
 * void DBInit(): Initializes the database object
 *
 * downloadData(): Called when the download button is pressed on either window, imports new data from neureset.
 *
 * Signals:
 * requestData(): Requests data, seems to be unused for now.
 * sessionDataDownloaded(): Also seems unused.
 * */

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
