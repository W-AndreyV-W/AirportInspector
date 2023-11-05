#ifndef DATABASECACHE_H
#define DATABASECACHE_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QVector>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>

#define CACHE_SIZE 130
#define SET_COLUMN_DAY 3

enum fieldsForScoreboardCache{
    arrival_c = 0,
    departure_c = 1
};

enum fieldsForQVectorCache{
    flight_no_c = 0,
    scheduled_departure_c = 1,
    airport_name_c = 2,
    scheduled_c = 3
};

class DatabaseCache : public QObject {

    Q_OBJECT

public:

    explicit DatabaseCache(QObject *parent = nullptr);
    ~DatabaseCache();

    void setAirportCalendar(QString _airport_code, QVector<QDate> _airport_calendar);
    void setAirportChart(QString _airport_code,  QDate date, QVector<QDate> chart_workload);
    bool selectDatabase(QDate _date, QString _airport_code);
    bool selectCalendar(QString _airport_code);
    bool selectChart(QDate date, QString _airport_code);
    bool boolManydays(QString _airport_code, QString date_txt);
    bool boolChart(QString _airport_code, QString date_txt);

signals:

    void sig_ScoreboardDay(QVector<QVector<QVector<QString>>> day_scoreboard);
    void sig_MaxMinDate(QVector<QDate> _airport_calendar);
    void sig_AirportChart(QDate _date, QVector<QDate> chart_workload);

public slots:

    void setAirportDatabase(QString _airport_code, QDate date, QVector<QVector<QVector<QString>>> _airport_scoreboard);

private:

    QMutex* mutex;

    QString airport_max_min;
    QVector<QDate> airport_data_set;

    QString airport_chart_set;
    QDate date_chart;
    QVector<QDate> chart_data_set;

    QString airport_scoreboard;
    QDate date_scoreboard;
    QVector<QVector<QVector<QString>>> airport_scoreboard_set;

    QDate select_date;
    QString select_airport_code;

    QMap<QString, QDateTime> airport_code_calendar;
    QMap<QDateTime, QVector<QDate>> airport_calendar;

    QMap<QString, QMap<QString, QDateTime>> airport_code_chart;
    QMap<QDateTime, QVector<QDate>> airport_chart;
    QMap<QDateTime, QString> time_airport_chart;

    QMap<QString, QMap<QString, QDateTime>> airport_database;
    QMap<QDateTime, QVector<QVector<QVector<QString>>>> database_cache;
    QMap<QDateTime, QString> time_airport_database;

    void selectVector(QMap<QDateTime, QVector<QVector<QVector<QString>>>>& cache,
                      QVector<QVector<QVector<QString>>>& day, QDateTime time_db, qint8 index);

};

#endif // DATABASECACHE_H
