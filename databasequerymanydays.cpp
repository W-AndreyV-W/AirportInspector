#include "databasequerymanydays.h"

DatabaseQueryManydays::DatabaseQueryManydays(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent)
    : QObject{parent} {

    sqlQuery = _sqlQuery;
    mutex = _mutex;
}

DatabaseQueryManydays::~DatabaseQueryManydays() {

}

void DatabaseQueryManydays::selectScoreboardManydays(QDate begin_date, QDate end_date, QString airport_code) {

    QMutexLocker locker(mutex);

    QDate date = begin_date;
    QString airport_select = "'" + airport_code + "'";
    QString end_select = "'" + end_date.toString("yyyy-MM-dd") + "'";
    QString begin_select = "'" + begin_date.toString("yyyy-MM-dd") + "'";
    QString select_a = select_scoreboard_day_a + airport_select + select_scoreboard_day_a_2
                       + begin_select + select_scoreboard_day_a_3 + end_select + select_scoreboard_day_a_4;
    QString select_d = select_scoreboard_day_d + airport_select + select_scoreboard_day_d_2
                       + begin_select + select_scoreboard_day_d_3 + end_select + select_scoreboard_day_d_4;

    if (airport_code != "") {

        QVector<QVector<QVector<QString>>> scoreboard(2);

        if(sqlQuery->exec(select_a)) {

            convertArray(scoreboard, arrival_month);
        }

        if(sqlQuery->exec(select_d)) {

            convertArray(scoreboard, departure_month);
        }

        emit sig_ScoreboardManydays(airport_code, date, scoreboard);
    }
}

void DatabaseQueryManydays::convertArray(QVector<QVector<QVector<QString>>>& scoreboard, qint8 index) {

    qsizetype size = sqlQuery->size();

    scoreboard[index].resize(size);

    for (qsizetype i = 0; sqlQuery->next(); i++) {

        scoreboard[index][i].resize(SET_COLUMN_MOMTH);
        scoreboard[index][i][flight_no_month] = sqlQuery->value(flight_no_month).toString();
        scoreboard[index][i][scheduled_departure_month] = sqlQuery->value(scheduled_departure_month).toDateTime().toString("dd:MM:yyyy hh:mm");
        scoreboard[index][i][airport_name_month] = sqlQuery->value(airport_name_month).toString();
        scoreboard[index][i][scheduled_month] = sqlQuery->value(scheduled_month).toDateTime().toString("dd:MM:yyyy");
    }
}
