#ifndef WORKLOADSCHEDULE_H
#define WORKLOADSCHEDULE_H

#include <QDialog>
#include <QDate>

#include "./library/qcustomplot.h"

#define MONTH 12
//#define STEPS 1000

//enum fieldsForScoreboardWorkload{
//    year_work = 0,
//    month_work = 1
//};

//enum fieldsForQVectorWorkload{
//    flight_no_work = 0,
//    scheduled_departure_work = 1,
//};

namespace Ui {
class WorkloadSchedule;
}

class WorkloadSchedule : public QDialog {

    Q_OBJECT

public:

    explicit WorkloadSchedule(QWidget *parent = nullptr);
    ~WorkloadSchedule();

    void printchart(QString _airport, QDate _date, QVector<QDate> _workload);
    void clearGraphic();

private slots:

    void on_pb_January_clicked();
    void on_pb_February_clicked();
    void on_pb_March_clicked();
    void on_pb_April_clicked();
    void on_pb_May_clicked();
    void on_pb_June_clicked();
    void on_pb_July_clicked();
    void on_pb_August_clicked();
    void on_pb_September_clicked();
    void on_pb_October_clicked();
    void on_pb_November_clicked();
    void on_pb_December_clicked();

    void on_pb_Close_clicked();

private:

    Ui::WorkloadSchedule *ui;

    QCPGraph* graphMonth;
    QCPBars* graphYear;

    QDate date;
    QString airport;
    QVector<QDate> workload;

    void printmonth(qint32 num);
    void switch_month(qint32 num);
};

#endif // WORKLOADSCHEDULE_H
