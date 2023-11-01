#ifndef DATABASEREQUEST_H
#define DATABASEREQUEST_H

#pragma once

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QThread>
#include <QThreadPool>
//#include <QRunnable>
#include <QtConcurrent>
#include <QSqlDatabase>


#include "databasequery.h"
#include "databasequeryairports.h"
#include "databasequerycalendar.h"
#include "databasequerychart.h"
#include "databasequeryday.h"
#include "databasequerymanydays.h"
#include "databasecache.h"

#define REQUEST_QUEUE 6

enum fieldsForRequestQueue{
    calendar = 0,
    request_day = 1,
    request_manydays = 2,
    request_chart = 3,
    request_plus_manydays = 4,
    request_minus_manydays = 5
};

class DatabaseRequest : public QObject {

    Q_OBJECT

public:

    explicit DatabaseRequest(QSqlDatabase& database, QObject* parent = nullptr);
    ~DatabaseRequest();

    void selectListAirports();
    void selectDateByAirport(QString _airport_code);
    void selectDatabaseSearch(QDate date);
    void selectChartWorkload();

signals:

    void sig_SetListAirports(QVector<QString> _airport_code, QVector<QString> _list_airports);
    void sig_MaxMinDate(QVector<QDate> max_min);
    void sig_Scoreboard(QVector<QVector<QVector<QString>>> scoreboard);
    void sig_ChartWorkload(QVector<QDate> chart_workload);

private slots:

    void listAirports(QVector<QString> _airport_code, QVector<QString> list_airports);
    void maxMinDate(QString _airport_code, QVector<QDate> max_min);
    void maxMinCache(QVector<QDate> max_min);
    void scoreboard(QVector<QVector<QVector<QString>>> day_scoreboard);
    void chartWorkload(QString airport,  QDate date_scoreboard, QVector<QDate> chart_workload);
    //void scoreboardManydays(QString _airport_code, QDate date_scoreboard, QVector<QVector<QVector<QString>>> _airport_scoreboard);

private:

    QMutex mutex;
    QThreadPool* pool;
    QSqlQuery* sqlQuery;

    DatabaseQuery* databaseQuery;
    DatabaseQueryAirports* databaseQueryAirports;
    DatabaseQueryCalendar* databaseQueryCalendar;
    DatabaseQueryChart* databaseQueryChart;
    DatabaseQueryDay* databaseQueryDay;
    DatabaseQueryManydays* databaseQueryManydays;
    DatabaseQueryManydays* databaseQueryManydaysPlus;
    DatabaseQueryManydays* databaseQueryManydaysMinus;
    DatabaseCache* databaseCache;

    QString airport_code;
    QDate current_date;
    QDate max_date;
    QDate min_date;
    //bool timeOut = true;

    QVector<bool> request_queue;
    QVector<QDate> set_begin_date;
    QVector<QDate> set_end_date;
    QVector<QString> set_airport_code;

    void requestQueue();
    void manyDays(QDate& begin_date, QDate& end_date, qint32 year_num, qint32 month_num);
    void manyDaysPlus(qint32 year_date, qint32 month_date);
    void manyDaysMinus(qint32 year_date, qint32 month_date);
//    void capturingQSqlDatabaseOn();
//    void capturingQSqlDatabaseOnOff();
};

#endif // DATABASEREQUEST_H
