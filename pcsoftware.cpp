#include "pcsoftware.h"
#include "session.h"
#include "pcui.h"

const QString PCSoftware::DATABASE_PATH = "/database/session.db";

PCSoftware::PCSoftware(QObject *parent)
    : QObject{parent}
{
    pcui = new PCui(nullptr, this);
    pcui->show();

    sessionDB = QSqlDatabase::addDatabase("QSQLITE");
    sessionDB.setDatabaseName("session.db");

    if (!sessionDB.open())
    {
        throw "Error: Database could not be opened";
    }

    if (!DBInit())
    {
        throw "Error: Database could not be initialized";
    }

    QSqlQuery query;
    int rowCount = 0;
    query.exec("SELECT * FROM sessions");

    // Create sessions based on session database table and add to sessionData
    while(query.next())
    {
        rowCount ++;

        Session* newSession = new Session();
        newSession->setDateTime(QDateTime::fromString(query.value(1).toString(), DATE_FORMAT));
        newSession->setBaselineBefore(std::stof(query.value(2).toString().toStdString()));
        newSession->setBaselineAfter(std::stof(query.value(3).toString().toStdString()));
        sessionData.push_back(newSession);
    }

    pcui->updateView(sessionData);
}

PCSoftware::~PCSoftware()
{
    for(int i=0; i<sessionData.size(); ++i){
        delete sessionData[i];
    }
    sessionData.clear();

    delete pcui;
}

void PCSoftware::handleDownloadButton()
{
    emit requestData();
}

void PCSoftware::printSessions() const
{
    for(int i=0; i<sessionData.size(); ++i){
        sessionData[i]->print();
    }
}

void PCSoftware::downloadData(QVector<Session *> sessions)
{
    sessionDB.transaction();
    QSqlQuery query;

    //make a deep copy of each session and store it in the session database and the sessionData vector
    for(int i=0; i<sessions.size(); ++i){
        if(!(sessions[i]->isUploaded())){
            // Insert the session into the database
            query.prepare("INSERT INTO sessions (session_date, baseline_before, baseline_after) VALUES (?, ?, ?);");
            query.addBindValue(sessions[i]->getDateTime().toString(DATE_FORMAT));
            query.addBindValue(sessions[i]->getBaselineBefore());
            query.addBindValue(sessions[i]->getBaselineAfter());
            query.exec();
            sessionDB.commit();

            Session* newSession = new Session(*(sessions[i]));
            sessionData.push_back(newSession);
            sessions[i]->setuploaded(true);
        }
    }
    //printSessions();
    pcui->updateView(sessionData);
}

bool PCSoftware::DBInit() {

    sessionDB.transaction();

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS sessions ( session_id INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT, session_date DATE NOT NULL, baseline_before FLOAT, baseline_after FLOAT);");

    return sessionDB.commit();
}
