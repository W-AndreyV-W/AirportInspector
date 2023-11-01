#ifndef DATABASEQUERYCHART_H
#define DATABASEQUERYCHART_H

#include "databasequery.h"

class DatabaseQueryChart : public DatabaseQuery {

    Q_OBJECT

public:

    DatabaseQueryChart(QSqlQuery* sqlQuery, QObject* parent);
    ~DatabaseQueryChart();

    void setData(QDate _begin_date, QDate _end_date, QString _airport_cod);
    void run() override;

signals:

    void sig_ChartWorkload(QString airport,  QDate date_scoreboard, QVector<QDate> chart_workload);

private:

    QDate begin_date;
    QDate end_date;
    QString airport_code;

//    QString select_scoreboard_year = "SELECT flight_no, scheduled_departure"
//                                      " FROM bookings.flights"
//                                      " WHERE (scheduled_departure::date => date(";
//    QString select_scoreboard_year_2 = ") AND scheduled_departure::date <= date(";
//    QString select_scoreboard_year_3 = ")) AND (departure_airport = ";
//    QString select_scoreboard_year_4 = " OR arrival_airport = ";
//    QString select_scoreboard_year_5 = ")";

//    QString select_scoreboard_year = "SELECT flight_no, scheduled_departure"
//                                      " FROM bookings.flights"
//                                      " WHERE (departure_airport = ";
//    QString select_scoreboard_year_2 = " AND arrival_airport = ";
//    QString select_scoreboard_year_3 = ") AND (scheduled_departure::date >= date(";
//    QString select_scoreboard_year_4 =  ") AND scheduled_departure::date < date(";
//    QString select_scoreboard_year_5 = ")) ORDER BY scheduled_departure ASC";


    QString select_chart_workload = "SELECT scheduled_departure::date"
                                      " FROM bookings.flights"
                                      " WHERE (departure_airport = ";
    QString select_chart_workload_2 = " OR arrival_airport = ";
    QString select_chart_workload_3 = ") AND (scheduled_departure::date >= date(";
    QString select_chart_workload_4 =  ") AND scheduled_departure::date <= date(";
    QString select_chart_workload_5 = ")) ORDER BY scheduled_departure ASC";

//    QString select_scoreboard_month = "SELECT count(flight_no), date_trunc('day', scheduled_departure) AS day"
//                                      " FROM bookings.flights"
//                                      " WHERE (scheduled_departure::date => date(";
//    QString select_scoreboard_month_2 = ") AND scheduled_departure::date <= date(";
//    QString select_scoreboard_month_3 = ")) AND (departure_airport = ";
//    QString select_scoreboard_month_4 = " OR arrival_airport = ";
//    QString select_scoreboard_month_5 = ") GROUP BY day"
//                                        " ORDER BY day ASC";

    void convertArray(QSqlQuery* sqlQuery, QVector<QDate>& scoreboard);
};

#endif // DATABASEQUERYCHART_H
