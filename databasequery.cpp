#include "databasequery.h"

DatabaseQuery::DatabaseQuery(QObject* parent)
    : QObject{parent}
{

    //sqlQuery = &_sqlQuery;

}

DatabaseQuery::~DatabaseQuery() {

}

void DatabaseQuery::run(QSqlQuery *sqlQuery) {

}
