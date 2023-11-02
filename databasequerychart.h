#ifndef DATABASEQUERYCHART_H
#define DATABASEQUERYCHART_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>
#include <QFuture>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include <QSqlError>

class DatabaseQueryChart : public QObject {

    Q_OBJECT

public:

    DatabaseQueryChart(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent);
    ~DatabaseQueryChart();

    void selectChartWorkload(QDate begin_date, QDate end_date, QString airport_code) ;

signals:

    void sig_ChartWorkload(QString airport,  QDate date_scoreboard, QVector<QDate> chart_workload);

private:

    QSqlQuery* sqlQuery;
    QMutex* mutex;

    QString select_chart_workload = "SELECT scheduled_departure::date"
                                      " FROM bookings.flights"
                                      " WHERE (departure_airport = ";
    QString select_chart_workload_2 = " OR arrival_airport = ";
    QString select_chart_workload_3 = ") AND (scheduled_departure::date >= date(";
    QString select_chart_workload_4 =  ") AND scheduled_departure::date <= date(";
    QString select_chart_workload_5 = ")) ORDER BY scheduled_departure ASC";

    void convertArray(QVector<QDate>& scoreboard);
};

#endif // DATABASEQUERYCHART_H
