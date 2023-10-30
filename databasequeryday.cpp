#include "databasequeryday.h"

DatabaseQueryDay::DatabaseQueryDay(QObject* parent)
    : DatabaseQuery{parent} {

}

DatabaseQueryDay::~DatabaseQueryDay() {

}

void DatabaseQueryDay::setData(QDate _date, QString _airport_code) {

    QMutexLocker locker(&mutex);

    date = _date;
    airport_code = _airport_code;
}

void DatabaseQueryDay::run(QSqlQuery *sqlQuery) {

    mutex.lock();

    if (airport_code != "") {

        QString airport = "'" + airport_code + "'";
        QString current_select = "'" + date.toString("yyyy-MM-dd") + "'";
        airport_code = "";

        mutex.unlock();

        QVector<QVector<QVector<QString>>> day_scoreboard(2);

        if (sqlQuery->exec(select_scoreboard_day_a + airport + select_scoreboard_day_a_2
                                            + current_select + select_scoreboard_day_a_3)) {

            convertArray(sqlQuery, day_scoreboard, arrival);
        }

        if (sqlQuery->exec(select_scoreboard_day_d + airport + select_scoreboard_day_d_2
                               + current_select + select_scoreboard_day_d_3)) {

            convertArray(sqlQuery, day_scoreboard, departure);
        }

        emit sig_ScoreboardDay(day_scoreboard);
    }
    else {

        mutex.unlock();
    }
}

void DatabaseQueryDay::convertArray(QSqlQuery* sqlQuery, QVector<QVector<QVector<QString>>>& scoreboard, qint8 index) {

    qsizetype size = sqlQuery->size();

    scoreboard[index].resize(size);

    for (qsizetype i = 0; sqlQuery->next(); i++) {

        scoreboard[index][i].resize(SET_COLUMN_DAY);
        scoreboard[index][i][flight_no] = sqlQuery->value(flight_no).toString();
        scoreboard[index][i][scheduled_departure] = sqlQuery->value(scheduled_departure).toDateTime().toString("dd:MM:yyyy hh:mm");
        scoreboard[index][i][airport_name] = sqlQuery->value(airport_name).toString();
    }
}
