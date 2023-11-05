#ifndef AIRPORTINSPECTOR_H
#define AIRPORTINSPECTOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QCloseEvent>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCloseEvent>


#include "databaseconnection.h"
#include "workloadschedule.h"
#include "databaserequest.h"

#define SETCOLUMN 3


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
    void selectChartWorkload(QDate date);
    void chartWorkload(QDate date, QVector<QDate> chart_workload);

    void on_calendarWidget_clicked(const QDate &date);
    void on_pb_AirportCongestion_clicked();
    void on_lw_Airport_clicked(const QModelIndex &index);

private:

    Ui::AirportInspector *ui;

    WorkloadSchedule* workloadSchedule = nullptr;
    DatabaseConnection* databaseConnection;
    DatabaseRequest* databaseRequest;
    QMessageBox* message;
    QTableWidgetItem* widgetItem;


    QDate date_max;
    QDate date_min;
    QDate date_airport;
    QString airport;
    QVector<QString> airport_code;
    QVector<QString> airport_name;

    void databaseConnectionError(DatabaseConnection& database);
};

#endif // AIRPORTINSPECTOR_H
