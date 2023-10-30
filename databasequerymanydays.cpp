#include "databasequerymanydays.h"

DatabaseQueryManydays::DatabaseQueryManydays(QObject* parent)
    : DatabaseQuery{parent} {


}

DatabaseQueryManydays::~DatabaseQueryManydays() {

}

void DatabaseQueryManydays::setData(QDate _begin_date, QDate _end_date, QString _airport_code) {

    QMutexLocker locker(&mutex);

    airport_code = _airport_code;
    begin_date = _begin_date;
    end_date = _end_date;
}

void DatabaseQueryManydays::run(QSqlQuery *sqlQuery) {

    mutex.lock();

    if (airport_code != "") {

        QString airport = "'" + airport_code + "'";
        QString end_select = "'" + end_date.toString("yyyy-MM-dd") + "'";
        QString begin_select = "'" + begin_date.toString("yyyy-MM-dd") + "'";
        QString _airport_code = airport_code;
        QDate _date = begin_date;
        //airport_code = "";

        mutex.unlock();

        QVector<QVector<QVector<QString>>> scoreboard(2);

        if(sqlQuery->exec(select_scoreboard_day_a + airport + select_scoreboard_day_a_2
                           + begin_select + select_scoreboard_day_a_3 + end_select + select_scoreboard_day_a_4)) {

            convertArray(sqlQuery, scoreboard, arrival);
        }

        if(sqlQuery->exec(select_scoreboard_day_d + airport + select_scoreboard_day_d_2
                           + begin_select + select_scoreboard_day_d_3 + end_select + select_scoreboard_day_d_4)) {

            convertArray(sqlQuery, scoreboard, departure);
        }

        emit sig_ScoreboardManydays(_airport_code, _date, scoreboard);
    }
    else {

        mutex.unlock();
    }
}

void DatabaseQueryManydays::convertArray(QSqlQuery *sqlQuery, QVector<QVector<QVector<QString>>>& scoreboard, qint8 index) {

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
