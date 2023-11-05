#ifndef DATABASEREQUEST_H
#define DATABASEREQUEST_H

#pragma once

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>
#include <QSqlDatabase>

#include "databasecache.h"
#include "databasequeryairports.h"

class DatabaseRequest : public QObject {

    Q_OBJECT

public:

    explicit DatabaseRequest(QSqlDatabase* database, QObject* parent = nullptr);
    ~DatabaseRequest();

    void selectListAirports();
    void selectDateByAirport(QString _airport_code);
    void selectDatabaseSearch(QDate date);
    void selectChartWorkload(QDate date);

signals:

    void sig_SetListAirports(QVector<QString> _airport_code, QVector<QString> _list_airports);
    void sig_MaxMinDate(QVector<QDate> max_min);
    void sig_Scoreboard(QVector<QVector<QVector<QString>>> scoreboard);
    void sig_ChartWorkload(QDate _date, QVector<QDate> chart_workload);

private slots:

    void maxMinDate(QString _airport_code, QVector<QDate> max_min);
    void chartWorkload(QString airport, QDate date_scoreboard, QVector<QDate> chart_workload);

private:

    QMutex* mutex;

    DatabaseCache* databaseCache;
    DatabaseQueryAirports* databaseQueryAirports;

    QString airport_code;
    QDate current_date;
    QDate chart_date;
    QDate max_date;
    QDate min_date;

    void manyDays(QDate date_fun, QDate small, QDate big, QString airport);
    void chartPlusMinus(QDate date_cache, QDate small, QDate big, QString airport);
};

#endif // DATABASEREQUEST_H
