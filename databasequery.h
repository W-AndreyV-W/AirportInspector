#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

#include <QObject>
#include <QVector>
#include <QString>
//#include <QMultiMap>
#include <QMutex>
//#include <QMutexLocker>
#include <QWaitCondition>
//#include <QThread>
//#include <QThreadPool>
#include <QtConcurrent>
#include <QRunnable>
#include <QAtomicPointer>
#include <QFuture>
#include <QMap>
//#include <QTableWidget>
//#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
//#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

#define SET_COLUMN_CHART 2
#define SET_COLUMN_DAY 3
#define SET_COLUMN_MOMTH 4
#define REQUEST_TIMEOUT 10

enum fieldsForChart{
    arrival = 0,
    departure = 1
};

enum fieldsForQVector{
    flight_no = 0,
    scheduled_departure = 1,
    airport_name = 2,
    scheduled = 3
};

enum fieldsForCalendar{
    max = 0,
    min = 1
};

class DatabaseQuery : public QObject {

    Q_OBJECT

public:

    explicit DatabaseQuery(QObject* parent = nullptr);
    ~DatabaseQuery();

    virtual void run(QSqlQuery* sqlQuery);

protected:

    //QSqlQuery* sqlQuery;
    QMutex mutex;
};

#endif // DATABASEQUERY_H
