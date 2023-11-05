#include "databasequeryairports.h"

DatabaseQueryAirports::DatabaseQueryAirports(QSqlDatabase* database, QObject* parent)
    : QObject{parent} {

    sqlQuery = new QSqlQuery(*database);
    mutex = new QMutex();
}

DatabaseQueryAirports::~DatabaseQueryAirports() {

    delete sqlQuery;
    delete mutex;
}

void DatabaseQueryAirports::selectListAirports() {

    QMutexLocker locker(mutex);

    QString select = select_list_airports;

    if (sqlQuery->exec(select)) {

        qsizetype size = sqlQuery->size();

        QVector<QString> airport_code(size, "");
        QVector<QString> list_airports(size, "");


        for (qsizetype i = 0; sqlQuery->next(); i++) {

            list_airports[i] = sqlQuery->value(0).toString() + " (" + sqlQuery->value(2).toString() + ")";
            airport_code[i] = sqlQuery->value(1).toString();
        }

        emit sig_listAirports(airport_code, list_airports);
    }

}

void DatabaseQueryAirports::selectMaxMinDate(QString airport_code) {

    QMutexLocker locker(mutex);

    QString airport_select = "'" + airport_code + "'";
    QString select = select_date + airport_select + select_date_2 + airport_select;

    if (airport_code != "") {

        if (sqlQuery->exec(select)) {

            sqlQuery->next();

            QVector<QDate> max_min(2);

            max_min[max] = sqlQuery->value(max).toDate();
            max_min[min] = sqlQuery->value(min).toDate();

            emit sig_MaxMinDate(airport_code, max_min);
        }
    }
}

void DatabaseQueryAirports::selectScoreboardDay(QDate date, QString airport_code) {

    QMutexLocker locker(mutex);

    QString airport_select = "'" + airport_code + "'";
    QString current_select = "'" + date.toString("yyyy-MM-dd") + "'";
    QString select_a = select_scoreboard_day_a + airport_select + select_scoreboard_day_a_2
                       + current_select + select_scoreboard_day_a_3;
    QString select_d = select_scoreboard_day_d + airport_select + select_scoreboard_day_d_2
                       + current_select + select_scoreboard_day_d_3;

    if (airport_code != "") {

        QVector<QVector<QVector<QString>>> day_scoreboard(2);

        convertArrayDay(day_scoreboard, select_a, arrival);

        convertArrayDay(day_scoreboard, select_d, departure);

        emit sig_ScoreboardDay(day_scoreboard);
    }
}

void DatabaseQueryAirports::selectScoreboardManydays(QDate begin_date, QDate end_date, QString airport_code) {

    QMutexLocker locker(mutex);

    QDate date = begin_date;
    QString airport_select = "'" + airport_code + "'";
    QString end_select = "'" + end_date.toString("yyyy-MM-dd") + "'";
    QString begin_select = "'" + begin_date.toString("yyyy-MM-dd") + "'";
    QString select_a = select_scoreboard_manyday_a + airport_select + select_scoreboard_manyday_a_2
                       + begin_select + select_scoreboard_manyday_a_3 + end_select + select_scoreboard_manyday_a_4;
    QString select_d = select_scoreboard_manyday_d + airport_select + select_scoreboard_manyday_d_2
                       + begin_select + select_scoreboard_manyday_d_3 + end_select + select_scoreboard_manyday_d_4;

    if (airport_code != "") {

        QVector<QVector<QVector<QString>>> scoreboard(2);

        convertArrayManydays(scoreboard, select_a, arrival);

        convertArrayManydays(scoreboard, select_d, departure);

        emit sig_ScoreboardManydays(airport_code, date, scoreboard);
    }
}

void DatabaseQueryAirports::selectChartWorkload(QDate begin_date, QDate end_date, QString airport_code) {

    QMutexLocker locker(mutex);

    QString airport_select = "'" + airport_code + "'";
    QString end_select = "'" + end_date.toString("yyyy-MM-dd") + "'";
    QString begin_select = "'" + begin_date.toString("yyyy-MM-dd") + "'";
    QString select = select_chart_workload + airport_select + select_chart_workload_2
                     + airport_select + select_chart_workload_3 + begin_select
                     + select_chart_workload_4 + end_select + select_chart_workload_5;
    QDate date = begin_date;

    if (airport_code != "") {

        QVector<QDate> workload;

        if(sqlQuery->exec(select)) {

            qsizetype size = sqlQuery->size();

            workload.resize(size);

            for (qsizetype i = 0; sqlQuery->next(); i++) {

                workload[i] = sqlQuery->value(0).toDate();
            }
        }

        emit sig_ChartWorkload(airport_code, date, workload);
    }
}

void DatabaseQueryAirports::convertArrayDay(QVector<QVector<QVector<QString>>>& scoreboard, QString select, qint8 index) {

    if (sqlQuery->exec(select)) {

        qsizetype size = sqlQuery->size();

        scoreboard[index].resize(size);

        for (qsizetype i = 0; sqlQuery->next(); i++) {

            scoreboard[index][i].resize(SET_COLUMN_DAY);
            scoreboard[index][i][flight_no] = sqlQuery->value(flight_no).toString();
            scoreboard[index][i][scheduled_departure] = sqlQuery->value(scheduled_departure).toDateTime().toString("dd:MM:yyyy hh:mm");
            scoreboard[index][i][airport_name] = sqlQuery->value(airport_name).toString();
        }
    }
}

void DatabaseQueryAirports::convertArrayManydays(QVector<QVector<QVector<QString>>>& scoreboard, QString select, qint8 index) {

    if(sqlQuery->exec(select)) {

        qsizetype size = sqlQuery->size();

        scoreboard[index].resize(size);

        for (qsizetype i = 0; sqlQuery->next(); i++) {

            scoreboard[index][i].resize(SET_COLUMN_MOMTH);
            scoreboard[index][i][flight_no] = sqlQuery->value(flight_no).toString();
            scoreboard[index][i][scheduled_departure] = sqlQuery->value(scheduled_departure).toDateTime().toString("dd:MM:yyyy hh:mm");
            scoreboard[index][i][airport_name] = sqlQuery->value(airport_name).toString();
            scoreboard[index][i][scheduled] = sqlQuery->value(scheduled).toDateTime().toString("dd:MM:yyyy");
        }
    }
}
