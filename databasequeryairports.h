#ifndef DATABASEQUERYAIRPORTS_H
#define DATABASEQUERYAIRPORTS_H

#include "databasequery.h"

class DatabaseQueryAirports : public DatabaseQuery {

    Q_OBJECT

public:

    explicit DatabaseQueryAirports(QObject* parent);
    ~DatabaseQueryAirports();

    void setListAirports(QSqlQuery* sqlQuery);

signals:

    void sig_listAirports(QVector<QString> airport_code, QVector<QString> list_airports);

private:

    QString select_list_airports = "SELECT airport_name->>'ru', airport_code, city->>'ru' "
                                   " FROM bookings.airports_data "
                                   " ORDER BY city->>'ru' ASC";
};

#endif // DATABASEQUERYAIRPORTS_H
