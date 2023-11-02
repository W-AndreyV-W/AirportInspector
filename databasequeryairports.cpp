#include "databasequeryairports.h"

DatabaseQueryAirports::DatabaseQueryAirports(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent)
    : QObject{parent} {

    sqlQuery = _sqlQuery;
    mutex = _mutex;
}

DatabaseQueryAirports::~DatabaseQueryAirports() {

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
