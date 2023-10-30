#ifndef DATABASEQUERYCHART_H
#define DATABASEQUERYCHART_H

#include "databasequery.h"

class DatabaseQueryChart : public DatabaseQuery {

    Q_OBJECT

public:

    DatabaseQueryChart(QObject* parent);
    ~DatabaseQueryChart();

    void setData(QDate _begin_date, QDate _end_date, QString _airport_cod);
    void run(QSqlQuery* sqlQuery) override;

signals:

    void sig_ScoreboardDay(QVector<QVector<QVector<QString>>> day_scoreboard);

private:

    QDate begin_date;
    QDate end_date;
    QString airport_code;

    QString select_scoreboard_year = "SELECT count(flight_no), date_trunc('month', scheduled_departure) AS month"
                                      " FROM bookings.flights f"
                                      " WHERE (scheduled_departure::date => date(";
    QString select_scoreboard_year_2 = " AND scheduled_departure::date <= date(";
    QString select_scoreboard_year_3 = ")) AND (departure_airport = ";
    QString select_scoreboard_year_4 = " OR arrival_airport = ";
    QString select_scoreboard_year_5 = ") GROUP BY month";

    QString select_scoreboard_month = "SELECT count(flight_no), date_trunc('day', scheduled_departure) AS day"
                                      " FROM bookings.flights f"
                                      " WHERE (scheduled_departure::date => date(";
    QString select_scoreboard_month_2 = " AND scheduled_departure::date <= date(";
    QString select_scoreboard_month_3 = ")) AND (departure_airport = ";
    QString select_scoreboard_month_4 = " OR arrival_airport = ";
    QString select_scoreboard_month_5 = ") GROUP BY day";


    void convertArray(QSqlQuery* sqlQuery, QVector<QVector<QVector<QString>>>& scoreboard, qint8 index);
};

#endif // DATABASEQUERYCHART_H
