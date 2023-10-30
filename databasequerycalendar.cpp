#include "databasequerycalendar.h"

DatabaseQueryCalendar::DatabaseQueryCalendar(QObject* parent)
    : DatabaseQuery{parent} {

    ;
}

DatabaseQueryCalendar::~DatabaseQueryCalendar() {

}

void DatabaseQueryCalendar::setData(QString _airport_code) {

    QMutexLocker locker(&mutex);

    airport_code =_airport_code;
}

void DatabaseQueryCalendar::run(QSqlQuery *sqlQuery) {

    mutex.lock();

    if (airport_code != "") {

        QString airport = "'" + airport_code + "'";
        //airport_code = "";

        mutex.unlock();

        if (sqlQuery->exec(select_date + airport + select_date_2 + airport)) {

            sqlQuery->next();

            QVector<QDate> max_min(2);

            max_min[max] = sqlQuery->value(max).toDate();
            max_min[min] = sqlQuery->value(min).toDate();

            emit sig_MaxMinDate(airport_code, max_min);
        }
    }
    else {

        mutex.unlock();
    }
}
