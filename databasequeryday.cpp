#include "databasequeryday.h"

DatabaseQueryDay::DatabaseQueryDay(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent)
    : QObject{parent} {

    sqlQuery = _sqlQuery;
    mutex = _mutex;
}

DatabaseQueryDay::~DatabaseQueryDay() {

}

void DatabaseQueryDay::selectScoreboardDay(QDate date, QString airport_code) {

    QMutexLocker locker(mutex);

    QString airport_select = "'" + airport_code + "'";
    QString current_select = "'" + date.toString("yyyy-MM-dd") + "'";
    QString select_a = select_scoreboard_day_a + airport_select + select_scoreboard_day_a_2
                       + current_select + select_scoreboard_day_a_3;
    QString select_d = select_scoreboard_day_d + airport_select + select_scoreboard_day_d_2
                       + current_select + select_scoreboard_day_d_3;

    if (airport_code != "") {

        QVector<QVector<QVector<QString>>> day_scoreboard(2);

        if (sqlQuery->exec(select_a)) {

            convertArray(day_scoreboard, arrival_day);
        }

        if (sqlQuery->exec(select_d)) {

            convertArray(day_scoreboard, departure_day);
        }

        emit sig_ScoreboardDay(day_scoreboard);
    }
}

void DatabaseQueryDay::convertArray(QVector<QVector<QVector<QString>>>& scoreboard, qint8 index) {

    qsizetype size = sqlQuery->size();

    scoreboard[index].resize(size);

    for (qsizetype i = 0; sqlQuery->next(); i++) {

        scoreboard[index][i].resize(SET_COLUMN_DAY);
        scoreboard[index][i][flight_no_day] = sqlQuery->value(flight_no_day).toString();
        scoreboard[index][i][scheduled_departure_day] = sqlQuery->value(scheduled_departure_day).toDateTime().toString("dd:MM:yyyy hh:mm");
        scoreboard[index][i][airport_name_day] = sqlQuery->value(airport_name_day).toString();
    }
}
