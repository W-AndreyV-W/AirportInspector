#include "databasequeryairports.h"

DatabaseQueryAirports::DatabaseQueryAirports(QSqlQuery* sqlQuery, QObject* parent)
    : DatabaseQuery{sqlQuery, parent} {
}

DatabaseQueryAirports::~DatabaseQueryAirports() {

}

void DatabaseQueryAirports::setListAirports() {

    //auto funList = QtConcurrent::run([&] () {

        if (sqlQuery->exec(select_list_airports)) {

            qsizetype size = sqlQuery->size();

            QVector<QString> airport_code(size, "");
            QVector<QString> list_airports(size, "");

            for (qsizetype i = 0; sqlQuery->next(); i++) {

                list_airports[i] = sqlQuery->value(0).toString() + " (" + sqlQuery->value(2).toString() + ")";
                airport_code[i] = sqlQuery->value(1).toString();
            }

            emit sig_listAirports(airport_code, list_airports);
        }
    //});
}
