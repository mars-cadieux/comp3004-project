#include "pcui.h"
#include "ui_pcui.h"
#include "pcsoftware.h"
#include "session.h"

PCui::PCui(QWidget *parent, PCSoftware* owner) :
    QWidget(parent),
    ui(new Ui::PCui)
{
    this->owner = owner;

    ui->setupUi(this);
    QWidget::setWindowTitle("PCui");

}

PCui::~PCui()
{
    delete ui;
}

//this is just a print function for now, will eventually use this data to populate the ui
void PCui::updateView(QVector<Session *> sessions)
{
    //set the number of rows in the table to the number of sessions stored
    int numSessions = sessions.size();
    ui->sessionDataTable->setRowCount(numSessions);

    //add the sessions as rows to the table, in reverse order, so that the newest session is the top row
    for(int i=numSessions-1; i>=0; --i){
        //make the cell for the session date/time and add it to column 0 of the table
        QTableWidgetItem* dt = new QTableWidgetItem((sessions[i]->getDateTime()).toString());
        ui->sessionDataTable->setItem(numSessions-i-1, 0, dt);

        //make the cell for the session's "before" baseline and add it to column 1 of the table
        QTableWidgetItem* bBefore = new QTableWidgetItem(QString::number(sessions[i]->getBaselineBefore()));
        ui->sessionDataTable->setItem(numSessions-i-1, 1, bBefore);

        //make the cell for the session's "after" baseline and add it to column 1 of the table
        QTableWidgetItem* bAfter = new QTableWidgetItem(QString::number(sessions[i]->getBaselineAfter()));
        ui->sessionDataTable->setItem(numSessions-i-1, 2, bAfter);

        //sessions[i]->print();
    }
}




