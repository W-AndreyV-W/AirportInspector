#ifndef DATABASEQUERYAIRPORTS_H
#define DATABASEQUERYAIRPORTS_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>
#include <QFuture>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDate>

#define SET_COLUMN_DAY 3
#define SET_COLUMN_MOMTH 4


enum fieldsForCalendar{
    max = 0,
    min = 1
};

enum fieldsForChartDay{
    arrival = 0,
    departure = 1
};

enum fieldsForQVectorDay{
    flight_no = 0,
    scheduled_departure = 1,
    airport_name = 2,
    scheduled = 3
};


class DatabaseQueryAirports : public QObject {

    Q_OBJECT

public:

    explicit DatabaseQueryAirports(QSqlDatabase* database, QObject* parent);
    ~DatabaseQueryAirports();

    void selectListAirports();
    void selectMaxMinDate(QString airport_code);
    void selectScoreboardDay(QDate date, QString airport_code);
    void selectScoreboardManydays(QDate begin_date, QDate end_date, QString airport_code);
    void selectChartWorkload(QDate begin_date, QDate end_date, QString airport_code);

signals:

    void sig_listAirports(QVector<QString> airport_code, QVector<QString> list_airports);
    void sig_MaxMinDate(QString airport_code, QVector<QDate> max_min);
    void sig_ScoreboardDay(QVector<QVector<QVector<QString>>> day_scoreboard);
    void sig_ScoreboardManydays(QString airport, QDate date_scoreboard, QVector<QVector<QVector<QString>>> _airport_scoreboard);
    void sig_ChartWorkload(QString airport,  QDate date_scoreboard, QVector<QDate> chart_workload);

private:

    QSqlQuery* sqlQuery;
    QMutex* mutex;

    QString select_list_airports = "SELECT airport_name->>'ru', airport_code, city->>'ru' "
                                   " FROM bookings.airports_data "
                                   " ORDER BY city->>'ru' ASC";

    QString select_date = "SELECT MAX(scheduled_departure::date), MIN(scheduled_departure::date)"
                          " FROM bookings.flights"
                          " WHERE departure_airport = ";
    QString select_date_2 = " OR arrival_airport = ";

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

    QString select_scoreboard_manyday_a = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru', scheduled_arrival::date"
                                      " FROM bookings.flights f"
                                      " JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport"
                                      " WHERE f.arrival_airport = ";
    QString select_scoreboard_manyday_a_2 = " AND (f.scheduled_arrival::date >= date(";
    QString select_scoreboard_manyday_a_3 = ") AND f.scheduled_arrival::date < date(";
    QString select_scoreboard_manyday_a_4 = ")) ORDER BY f.scheduled_arrival ASC";

    QString select_scoreboard_manyday_d = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru', scheduled_departure::date"
                                          " FROM bookings.flights f"
                                          " JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport"
                                          " WHERE f.departure_airport = ";
    QString select_scoreboard_manyday_d_2 = " AND (f.scheduled_departure::date >= date(";
    QString select_scoreboard_manyday_d_3 =  ") AND f.scheduled_departure::date < date(";
    QString select_scoreboard_manyday_d_4 = ")) ORDER BY f.scheduled_departure ASC";

    QString select_chart_workload = "SELECT scheduled_departure::date"
                                    " FROM bookings.flights"
                                    " WHERE (departure_airport = ";
    QString select_chart_workload_2 = " OR arrival_airport = ";
    QString select_chart_workload_3 = ") AND (scheduled_departure::date >= date(";
    QString select_chart_workload_4 =  ") AND scheduled_departure::date <= date(";
    QString select_chart_workload_5 = ")) ORDER BY scheduled_departure ASC";

    void convertArrayDay(QVector<QVector<QVector<QString>>>& scoreboard, QString select, qint8 index);
    void convertArrayManydays(QVector<QVector<QVector<QString>>>& scoreboard, QString select, qint8 index);
};

#endif // DATABASEQUERYAIRPORTS_H
