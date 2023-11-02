#ifndef DATABASEQUERYDAY_H
#define DATABASEQUERYDAY_H

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

#define SET_COLUMN_DAY 3

enum fieldsForChartDay{
    arrival_day = 0,
    departure_day = 1
};

enum fieldsForQVectorDay{
    flight_no_day = 0,
    scheduled_departure_day = 1,
    airport_name_day= 2,
    scheduled_day = 3
};

class DatabaseQueryDay : public QObject {

    Q_OBJECT

public:

    DatabaseQueryDay(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent);
    ~DatabaseQueryDay();

    void selectScoreboardDay(QDate date, QString airport_code) ;

signals:

    void sig_ScoreboardDay(QVector<QVector<QVector<QString>>> day_scoreboard);

private:

    QSqlQuery* sqlQuery;
    QMutex* mutex;

    QString select_scoreboard_day_a = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru'"
                                      " FROM bookings.flights f"
                                      " JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport"
                                      " WHERE f.arrival_airport = ";
    QString select_scoreboard_day_a_2 = " AND f.scheduled_arrival::date = date(";
    QString select_scoreboard_day_a_3 = ") ORDER BY f.scheduled_arrival ASC";

    QString select_scoreboard_day_d = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru'"
                                      " FROM bookings.flights f"
                                      " JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport"
                                      " WHERE f.departure_airport = ";
    QString select_scoreboard_day_d_2 = " AND f.scheduled_departure::date = date(";
    QString select_scoreboard_day_d_3 = ") ORDER BY f.scheduled_departure ASC";

    void convertArray(QVector<QVector<QVector<QString>>>& scoreboard, qint8 index);
};

#endif // DATABASEQUERYDAY_H
