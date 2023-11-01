#ifndef DATABASEQUERYDAY_H
#define DATABASEQUERYDAY_H

#include "databasequery.h"

class DatabaseQueryDay : public DatabaseQuery {

    Q_OBJECT

public:

    DatabaseQueryDay(QSqlQuery* sqlQuery, QObject* parent);
    ~DatabaseQueryDay();

    void setData(QDate _date, QString _airport_code);
    void run() override;

signals:

    void sig_ScoreboardDay(QVector<QVector<QVector<QString>>> day_scoreboard);

private:

    QDate date;
    QString airport_code;

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

    void convertArray(QSqlQuery* sqlQuery, QVector<QVector<QVector<QString>>>& scoreboard, qint8 index);
};

#endif // DATABASEQUERYDAY_H
