#ifndef DATABASEQUERYCALENDAR_H
#define DATABASEQUERYCALENDAR_H

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

enum fieldsForCalendar{
    max = 0,
    min = 1
};

class DatabaseQueryCalendar : public QObject {

    Q_OBJECT

public:

    DatabaseQueryCalendar(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent);
    ~DatabaseQueryCalendar();

    void selectMaxMinDate(QString airport_code);

signals:

    void sig_MaxMinDate(QString airport_code, QVector<QDate> max_min);

private:

    QSqlQuery* sqlQuery;
    QMutex* mutex;

    QString select_date = "SELECT MAX(scheduled_departure::date), MIN(scheduled_departure::date)"
                          " FROM bookings.flights"
                          " WHERE departure_airport = ";
    QString select_date_2 = " OR arrival_airport = ";
};

#endif // DATABASEQUERYCALENDAR_H
