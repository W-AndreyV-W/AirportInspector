#ifndef DATABASEQUERYAIRPORTS_H
#define DATABASEQUERYAIRPORTS_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>
#include <QFuture>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include <QSqlError>

class DatabaseQueryAirports : public QObject {

    Q_OBJECT

public:

    explicit DatabaseQueryAirports(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent);
    ~DatabaseQueryAirports();

    void selectListAirports();

signals:

    void sig_listAirports(QVector<QString> airport_code, QVector<QString> list_airports);

private:

    QSqlQuery* sqlQuery;
    QMutex* mutex;

    QString select_list_airports = "SELECT airport_name->>'ru', airport_code, city->>'ru' "
                                   " FROM bookings.airports_data "
                                   " ORDER BY city->>'ru' ASC";
};

#endif // DATABASEQUERYAIRPORTS_H
