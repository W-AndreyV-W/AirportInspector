#include "databaserequest.h"

DatabaseRequest::DatabaseRequest(QSqlDatabase& database, QObject* parent)
    : QObject{parent}
{
//    lockpool = new QWaitCondition();
//    lockmap = new QWaitCondition();

    sqlQuery = new QSqlQuery(database);

    pool = new QThreadPool(this);

    databaseQuery = new DatabaseQuery(this);
    databaseQueryAirports = new DatabaseQueryAirports(this);
    databaseQueryCalendar = new DatabaseQueryCalendar(this);
    //databaseQueryChart = new DatabaseQueryChart(sqlQuery, this);
    databaseQueryDay = new DatabaseQueryDay(this);
    databaseQueryManydays = new DatabaseQueryManydays(this);
    databaseQueryManydaysPlus = new DatabaseQueryManydays(this);
    databaseQueryManydaysMinus  = new DatabaseQueryManydays(this);
    databaseCache = new DatabaseCache(this);

    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_listAirports, this, &DatabaseRequest::listAirports);
    QObject::connect(databaseQueryCalendar, &DatabaseQueryCalendar::sig_MaxMinDate, this, &DatabaseRequest::maxMinDate);
    QObject::connect(databaseCache, &DatabaseCache::sig_MaxMinDate, this, &DatabaseRequest::maxMinCache);
    QObject::connect(databaseQueryDay, &DatabaseQueryDay::sig_ScoreboardDay, this, &DatabaseRequest::scoreboard);
    QObject::connect(databaseCache, &DatabaseCache::sig_ScoreboardDay, this, &DatabaseRequest::scoreboard);
    QObject::connect(databaseQueryManydays, &DatabaseQueryManydays::sig_ScoreboardManydays, databaseCache, &DatabaseCache::setAirportDatabase);
    QObject::connect(databaseQueryManydaysPlus, &DatabaseQueryManydays::sig_ScoreboardManydays, databaseCache, &DatabaseCache::setAirportDatabase);
    QObject::connect(databaseQueryManydaysMinus, &DatabaseQueryManydays::sig_ScoreboardManydays, databaseCache, &DatabaseCache::setAirportDatabase);

    request_queue.resize(REQUEST_QUEUE, false);

    //requestQueue();
}

DatabaseRequest::~DatabaseRequest() {

}

void DatabaseRequest::setListAirports() {

    databaseQueryAirports->setListAirports(sqlQuery);
}

void DatabaseRequest::selectionDateByAirport(QString _airport_code) {

    //QMutexLocker locker(&mutex);

    airport_code = _airport_code;

    if (!databaseCache->selectCalendar(_airport_code)) {

        databaseQueryCalendar->setData(_airport_code);

        //request_queue[calendar] = true;

        //auto funCalendar = QtConcurrent::run(pool, [&] () {

            databaseQueryCalendar->run(sqlQuery);
        //});
    }
}

void DatabaseRequest::databaseSearch(QDate date) {

    //QMutexLocker locker(&mutex);

    current_date = date;

    qint32 month_date = date.month();
    qint32 year_date = date.year();

    if (!databaseCache->selectDatabase(date, airport_code)) {

        databaseQueryDay->setData(date, airport_code);

        //auto funDay = QtConcurrent::run(pool, [&] () {

            databaseQueryDay->run(sqlQuery);
        //});

        QDate begin_date;
        QDate end_date;

        manyDays(begin_date, end_date, year_date, month_date);

        databaseQueryManydays->setData(begin_date, end_date, airport_code);

        //auto funManydays = QtConcurrent::run(pool, [&] () {

            databaseQueryManydays->run(sqlQuery);
        //});
    }

    manyDaysPlus(year_date, month_date);
    manyDaysMinus(year_date, month_date);
}

void DatabaseRequest::listAirports(QVector<QString> _airport_code, QVector<QString> _list_airports) {

    QVector<QString> airport_code = _airport_code;
    QVector<QString> list_airports = _list_airports;

    emit sig_SetListAirports(airport_code, list_airports);
}

void DatabaseRequest::maxMinDate(QString _airport_code, QVector<QDate> max_min) {

    QMutexLocker locker(&mutex);

    if (airport_code == _airport_code) {

        max_date = max_min.value(max);
        min_date = max_min.value(min);

        emit sig_MaxMinDate(max_min);
    }

    databaseCache->setAirportCalendar(_airport_code, max_min);
}

void DatabaseRequest::maxMinCache(QVector<QDate> max_min) {

    emit sig_MaxMinDate(max_min);
}

void DatabaseRequest::scoreboard(QVector<QVector<QVector<QString>>> day_scoreboard) {

    emit sig_Scoreboard(day_scoreboard);
}

void DatabaseRequest::manyDays(QDate& begin_date, QDate& end_date, qint32 year_num, qint32 month_num) {

    //QMutexLocker locker(&mutex);

    begin_date = QDate(year_num, month_num, 1);


    if (month_num != 12) {

        end_date = QDate(year_num, month_num + 1, 1);
    }
    else {

        end_date = QDate(year_num + 1, 1, 1);
    }

    if (begin_date < min_date) {

        begin_date = min_date;
    }

    if (end_date > max_date) {

        end_date = max_date.addDays(1);
    }
}

void DatabaseRequest::manyDaysPlus(qint32 year_date, qint32 month_date) {

    ++month_date;

    if (month_date > 12) {

        month_date = 1;
        ++year_date;
    }

    qint32 year_month = year_date * 100 + month_date;
    qint32 year_month_max = max_date.year() * 100 + max_date.month();

    if (year_month_max >= year_month) {

        QString date_manydays = QString::number(year_date) + ":" + QString::number(month_date);

        if (!databaseCache->boolManydays(airport_code, date_manydays)) {

            QDate begin_date;
            QDate end_date;

            manyDays(begin_date, end_date, year_date, month_date);

            databaseQueryManydaysPlus->setData(begin_date, end_date, airport_code);

            //auto funPlusManydays = QtConcurrent::run(pool, [&] () {

                databaseQueryManydaysPlus->run(sqlQuery);
            //});
        }
    }
}

void DatabaseRequest::manyDaysMinus(qint32 year_date, qint32 month_date) {

    --month_date;

    if (month_date < 1) {

        month_date = 12;
        --year_date;
    }

    qint32 year_month = year_date * 100 + month_date;
    qint32 year_month_min = min_date.year() * 100 + min_date.month();

    if (year_month >= year_month_min) {

        QString date_manydays = QString::number(year_date) + ":" + QString::number(month_date);

        if (!databaseCache->boolManydays(airport_code, date_manydays)) {

            QDate begin_date;
            QDate end_date;

            manyDays(begin_date, end_date, year_date, month_date);

            databaseQueryManydaysMinus->setData(begin_date, end_date, airport_code);

            //auto funMinusManydays = QtConcurrent::run(pool, [&] () {

                databaseQueryManydaysMinus->run(sqlQuery);
            //});
        }
    }
}
