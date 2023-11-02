#ifndef DATABASEREQUEST_H
#define DATABASEREQUEST_H

#pragma once

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QThread>
#include <QtConcurrent>
#include <QSqlDatabase>


#include "databasecache.h"
#include "databasequeryairports.h"
#include "databasequerycalendar.h"
#include "databasequerychart.h"
#include "databasequeryday.h"
#include "databasequerymanydays.h"

#define REQUEST_TIMEOUT 100

class DatabaseRequest : public QObject {

    Q_OBJECT

public:

    explicit DatabaseRequest(QSqlDatabase* database, QObject* parent = nullptr);
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

private:

    QMutex* mutex;
    QMutex* mutexTimeOut;
    QSqlQuery* sqlQuery;

    DatabaseCache* databaseCache;
    DatabaseQueryAirports* databaseQueryAirports;
    DatabaseQueryCalendar* databaseQueryCalendar;
    DatabaseQueryDay* databaseQueryDay;
    DatabaseQueryManydays* databaseQueryManydays;
    DatabaseQueryChart* databaseQueryChart;

    QString airport_code;
    QDate current_date;
    QDate max_date;
    QDate min_date;

    void requestQueue();
    void manyDays(QDate& begin, QDate& end, qint32 year_num, qint32 month_num);
    void manyDaysPlus(QDate& begin_date, QDate& end_date, qint32 year_date, qint32 month_date);
    void manyDaysMinus(QDate& begin_date, QDate& end_date, qint32 year_date, qint32 month_date);
};

#endif // DATABASEREQUEST_H
