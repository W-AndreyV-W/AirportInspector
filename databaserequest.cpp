#include "databaserequest.h"

DatabaseRequest::DatabaseRequest(QSqlDatabase* database, QObject* parent)
    : QObject{parent}
{

    sqlQuery = new QSqlQuery(*database);
    mutex = new QMutex();
    mutexTimeOut = new QMutex();

    databaseCache = new DatabaseCache(this);
    databaseQueryAirports = new DatabaseQueryAirports(sqlQuery, mutexTimeOut, parent);
    databaseQueryCalendar = new DatabaseQueryCalendar(sqlQuery, mutexTimeOut, parent);
    databaseQueryDay = new DatabaseQueryDay(sqlQuery, mutexTimeOut, parent);
    databaseQueryManydays = new DatabaseQueryManydays(sqlQuery, mutexTimeOut, parent);
    databaseQueryChart = new DatabaseQueryChart(sqlQuery, mutexTimeOut, parent);

    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_listAirports, this, &DatabaseRequest::listAirports);
    QObject::connect(databaseQueryCalendar, &DatabaseQueryCalendar::sig_MaxMinDate, this, &DatabaseRequest::maxMinDate);
    QObject::connect(databaseCache, &DatabaseCache::sig_MaxMinDate, this, &DatabaseRequest::maxMinCache);
    QObject::connect(databaseQueryDay, &DatabaseQueryDay::sig_ScoreboardDay, this, &DatabaseRequest::scoreboard);
    QObject::connect(databaseCache, &DatabaseCache::sig_ScoreboardDay, this, &DatabaseRequest::scoreboard);
    QObject::connect(databaseQueryManydays, &DatabaseQueryManydays::sig_ScoreboardManydays, databaseCache, &DatabaseCache::setAirportDatabase);
    QObject::connect(databaseQueryChart, &DatabaseQueryChart::sig_ChartWorkload, this, &DatabaseRequest::chartWorkload);
}

DatabaseRequest::~DatabaseRequest() {

    delete mutexTimeOut;
    delete mutex;
    delete sqlQuery;
}

void DatabaseRequest::selectListAirports() {

    QMutexLocker locker(mutex);

    databaseQueryAirports->selectListAirports();
}

void DatabaseRequest::selectDateByAirport(QString _airport_code) {

    QMutexLocker locker(mutex);

    airport_code = _airport_code;

    if (!databaseCache->selectCalendar(_airport_code)) {

        auto funCalendar = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QString airport = airport_code;

            databaseQueryCalendar->selectMaxMinDate(airport);
        });
    }
}

void DatabaseRequest::selectDatabaseSearch(QDate date) {

    QMutexLocker locker(mutex);

    current_date = date;

    qint32 month_date = current_date.month();
    qint32 year_date = current_date.year();
    QDate begin_date;
    QDate end_date;


    if (!databaseCache->selectDatabase(current_date, airport_code)) {

        databaseQueryDay->selectScoreboardDay(current_date, airport_code);

        auto funDay = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            qint32 month_date = current_date.month();
            qint32 year_date = current_date.year();
            QDate begin_date;
            QDate end_date;

            manyDays(begin_date , end_date, year_date, month_date);

            databaseQueryManydays->selectScoreboardManydays(begin_date , end_date, airport_code);
        });
    }

        auto funDay = QtConcurrent::run([&] () {

        QMutexLocker locker(mutex);

        qint32 month_date = current_date.month();
        qint32 year_date = current_date.year();
        QDate begin_date;
        QDate end_date;

        manyDaysPlus(begin_date , end_date, year_date, month_date);
        manyDaysMinus(begin_date , end_date, year_date, month_date);
    });
}

void DatabaseRequest::selectChartWorkload() {

    auto funManyChar = QtConcurrent::run([&] () {

        QMutexLocker locker(mutex);
        QDate begin_date;
        QDate end_date;

        begin_date = QDate(current_date.year(), 1, 1);
        end_date = QDate(current_date.year(), 12, 31);

        if (begin_date < min_date) {

            begin_date = min_date;
        }

        if (end_date > max_date) {

            end_date = max_date;
        }

        databaseQueryChart->selectChartWorkload(begin_date, end_date, airport_code);
    });
}

void DatabaseRequest::listAirports(QVector<QString> _airport_code, QVector<QString> _list_airports) {

    emit sig_SetListAirports(_airport_code, _list_airports);
}

void DatabaseRequest::maxMinDate(QString _airport_code, QVector<QDate> max_min) {

    QMutexLocker locker(mutex);

    if (airport_code == _airport_code) {

        max_date = max_min.value(max);
        min_date = max_min.value(min);

        locker.unlock();

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

void DatabaseRequest::chartWorkload(QString airport, QDate date_scoreboard, QVector<QDate> chart_workload) {

    QMutexLocker locker(mutex);

    if (airport == airport_code) {

        locker.unlock();

        emit sig_ChartWorkload(chart_workload);
    }
}

void DatabaseRequest::manyDays(QDate& begin_date, QDate& end_date, qint32 year_num, qint32 month_num) {

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

void DatabaseRequest::manyDaysPlus(QDate& begin_date, QDate& end_date, qint32 year_date, qint32 month_date) {

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

            manyDays(begin_date , end_date, year_date, month_date);

            databaseQueryManydays->selectScoreboardManydays(begin_date , end_date, airport_code);
        }
    }
}

void DatabaseRequest::manyDaysMinus(QDate& begin_date, QDate& end_date, qint32 year_date, qint32 month_date) {

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

            manyDays(begin_date , end_date, year_date, month_date);

            databaseQueryManydays->selectScoreboardManydays(begin_date , end_date, airport_code);
        }
    }
}
