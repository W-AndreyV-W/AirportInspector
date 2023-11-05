#ifndef WORKLOADSCHEDULE_H
#define WORKLOADSCHEDULE_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QDate>

#include "./library/qcustomplot.h"

#define MONTH 12

namespace Ui {
class WorkloadSchedule;
}

class WorkloadSchedule : public QDialog {

    Q_OBJECT

public:

    explicit WorkloadSchedule(QString _airport, QDate _date, QDate _date_max, QDate _date_min, QVector<QDate> chart_workload, QWidget *parent = nullptr);
    ~WorkloadSchedule();

    void printYear(QVector<QDate> _workload);
    void setWorkloadSchedule(QString _airport, QDate _date, QDate _date_max, QDate _date_min, QVector<QDate> chart_workload);

signals:

    void sig_selectChartWorkload(QDate date);

private slots:

    void on_pb_Close_clicked();
    void on_lw_YearUp_itemClicked(QListWidgetItem *item);
    void on_lw_YearDown_itemClicked(QListWidgetItem *item);
    void on_lw_MonthYearUp_itemClicked(QListWidgetItem *item);
    void on_lw_MonthYearDown_itemClicked(QListWidgetItem *item);
    void on_lw_MonthUp_clicked(const QModelIndex &index);
    void on_lw_MonthDown_clicked(const QModelIndex &index);

private:

    Ui::WorkloadSchedule *ui;

    QCPGraph* graphMonth;
    QCPBars* graphYear;

    qint32 month = 1;
    QDate date_max;
    QDate date_min;
    QDate date;
    QString airport;
    QVector<qint32> month_up;
    QVector<qint32> month_down;
    QVector<double> workload_year;
    QVector<QVector<double>> workload_month;

    void printmonth(qint32 month_print);
    QString switch_month(qint32 num);
};

#endif // WORKLOADSCHEDULE_H
