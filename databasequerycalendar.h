#ifndef DATABASEQUERYCALENDAR_H
#define DATABASEQUERYCALENDAR_H

#include "databasequery.h"

class DatabaseQueryCalendar : public DatabaseQuery {

    Q_OBJECT

public:

    DatabaseQueryCalendar(QSqlQuery* sqlQuery, QObject* parent);
    ~DatabaseQueryCalendar();

    void setData(QString _airport_code);
    void run() override;

signals:

    void sig_MaxMinDate(QString _airport_code, QVector<QDate> max_min);

private:

    QString airport_code;

    QString select_date = "SELECT MAX(scheduled_departure::date), MIN(scheduled_departure::date)"
                          " FROM bookings.flights"
                          " WHERE departure_airport = ";
    QString select_date_2 = " OR arrival_airport = ";
};

#endif // DATABASEQUERYCALENDAR_H
