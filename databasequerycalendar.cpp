#include "databasequerycalendar.h"

DatabaseQueryCalendar::DatabaseQueryCalendar(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent)
    : QObject{parent} {

    sqlQuery = _sqlQuery;
    mutex = _mutex;
}

DatabaseQueryCalendar::~DatabaseQueryCalendar() {

}


void DatabaseQueryCalendar::selectMaxMinDate(QString airport_code) {

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
