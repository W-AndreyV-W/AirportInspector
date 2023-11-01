#include "databasequery.h"

DatabaseQuery::DatabaseQuery(QSqlQuery* _sqlQuery, QObject* parent)
    : QObject{parent}
{

    sqlQuery = _sqlQuery;

}

DatabaseQuery::~DatabaseQuery() {

}

void DatabaseQuery::run() {

}
