#ifndef AIRPORTINSPECTOR_H
#define AIRPORTINSPECTOR_H

#include <QMainWindow>
#include <QMessageBox>
//#include <QtConcurrent>
#include <QThread>
#include <QCloseEvent>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>


#include "databaseconnection.h"
#include "workloadschedule.h"
#include "databaserequest.h"

#define ZIRO_DATE 1900, 1, 1

QT_BEGIN_NAMESPACE
namespace Ui { class AirportInspector; }
QT_END_NAMESPACE

class AirportInspector : public QMainWindow
{

    Q_OBJECT

public:

    AirportInspector(QWidget *parent = nullptr);
    ~AirportInspector();

private slots:

    void databaseConnectionStatusDay(bool status);
    void printListAirports(QVector<QString> _airport_code, QVector<QString> _list_airports);
    void maxMinDate(QVector<QDate> max_min);
    void Scoreboard(QVector<QVector<QVector<QString>>> scoreboard);

    void on_lw_Airport_itemDoubleClicked(QListWidgetItem *item);

    void on_calendarWidget_clicked(const QDate &date);

private:

    Ui::AirportInspector *ui;
    WorkloadSchedule* waorkloadSchedule;
    DatabaseConnection* databaseConnection;
    DatabaseRequest* databaseRequest;
    QMessageBox* message;
    QTableWidgetItem* widgetItem;

    QVector<QString> airport_code;

    void databaseConnectionError(DatabaseConnection& database);

};

#endif // AIRPORTINSPECTOR_H
