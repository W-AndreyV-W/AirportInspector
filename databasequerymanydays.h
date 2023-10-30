#ifndef DATABASEQUERYMANYDAYS_H
#define DATABASEQUERYMANYDAYS_H

#include "databasequery.h"

class DatabaseQueryManydays : public DatabaseQuery {

    Q_OBJECT

public:

    DatabaseQueryManydays(QObject* parent);
    ~DatabaseQueryManydays();

    void setData(QDate _begin_date, QDate _end_date, QString _airport_cod);
    void run(QSqlQuery* sqlQuery) override;

signals:

    void sig_ScoreboardManydays(QString airport, QDate date_scoreboard, QVector<QVector<QVector<QString>>> _airport_scoreboard);

private:

    QDate begin_date;
    QDate end_date;
    QString airport_code;

    QString select_scoreboard_day_d = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru', scheduled_departure::date"
                                      " FROM bookings.flights f"
                                      " JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport"
                                      " WHERE f.departure_airport = ";
    QString select_scoreboard_day_d_2 = " AND (f.scheduled_departure::date >= date(";
    QString select_scoreboard_day_d_3 =  ") AND f.scheduled_departure::date < date(";
    QString select_scoreboard_day_d_4 = ")) ORDER BY f.scheduled_departure ASC";

    QString select_scoreboard_day_a = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru', scheduled_arrival::date"
                                      " FROM bookings.flights f"
                                      " JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport"
                                      " WHERE f.arrival_airport = ";
    QString select_scoreboard_day_a_2 = " AND (f.scheduled_arrival::date >= date(";
    QString select_scoreboard_day_a_3 = ") AND f.scheduled_arrival::date < date(";
    QString select_scoreboard_day_a_4 = ")) ORDER BY f.scheduled_arrival ASC";

    void convertArray(QSqlQuery* sqlQuery, QVector<QVector<QVector<QString>>>& scoreboard, qint8 index);
};

#endif // DATABASEQUERYMANYDAYS_H
