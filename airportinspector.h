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
#include <QCloseEvent>


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
    void ChartWorkload(QVector<QDate> chart_workload);

    void on_calendarWidget_clicked(const QDate &date);
    void on_pb_AirportCongestion_clicked();
    void on_lw_Airport_clicked(const QModelIndex &index);

private:

    Ui::AirportInspector *ui;

    WorkloadSchedule* waorkloadSchedule;
    DatabaseConnection* databaseConnection;
    DatabaseRequest* databaseRequest;
    QMessageBox* message;
    QTableWidgetItem* widgetItem;

    QDate date;
    QString airport;
    QVector<QString> airport_code;
    QVector<QString> airport_name;
    bool close_event = true;

    void databaseConnectionError(DatabaseConnection& database);
    void closeEvent (QCloseEvent *event) override;

};

#endif // AIRPORTINSPECTOR_H
