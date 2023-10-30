#ifndef DATABASECACHE_H
#define DATABASECACHE_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QVector>
#include <QMap>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>

#define CACHE_SIZE 52
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
    bool selectDatabase(QDate _date, QString _airport_code);
    bool selectCalendar(QString _airport_code);
    bool boolManydays(QString _airport_code, QString date_manydays);

signals:

    void sig_ScoreboardDay(QVector<QVector<QVector<QString>>> day_scoreboard);
    void sig_MaxMinDate(QVector<QDate> _airport_calendar);

public slots:

    void setAirportDatabase(QString _airport_code, QDate date, QVector<QVector<QVector<QString>>> _airport_scoreboard);

private:

    QMutex mutex;

    QDate date_set_scoreboard;
    QDate select_date;
    QString select_airport_code;
    QString airport_code_set_calendar;
    QString airport_code_set_scoreboard;

    QVector<QDate> airport_calendar_set;
    QVector<QVector<QVector<QString>>> airport_scoreboard_set;

    QMap<QString, QDateTime> airport_code;
    QMap<QString, QVector<QDate>> airport_calendar;
    QMap<QString, QMap<QString, QDateTime>> airport_database;
    QMap<QDateTime, QVector<QVector<QVector<QString>>>> database_cache;
    QMap<QDateTime, QString> time_airport_code;

    void selectVector(QMap<QDateTime, QVector<QVector<QVector<QString>>>>& cache,
                      QVector<QVector<QVector<QString>>>& day, QDateTime time_db, qint8 index);

};

#endif // DATABASECACHE_H
