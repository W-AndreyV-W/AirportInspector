#include "databaserequest.h"

DatabaseRequest::DatabaseRequest(QSqlDatabase* database, QObject* parent)
    : QObject{parent}
{

    mutex = new QMutex();

    databaseCache = new DatabaseCache(parent);
    databaseQueryAirports = new DatabaseQueryAirports(database, parent);

    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_listAirports, this, &DatabaseRequest::sig_SetListAirports);
    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_MaxMinDate, this, &DatabaseRequest::maxMinDate);
    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_ScoreboardDay, this, &DatabaseRequest::sig_Scoreboard);
    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_ScoreboardManydays, databaseCache, &DatabaseCache::setAirportDatabase);
    QObject::connect(databaseQueryAirports, &DatabaseQueryAirports::sig_ChartWorkload, this, &DatabaseRequest::chartWorkload);
    QObject::connect(databaseCache, &DatabaseCache::sig_MaxMinDate, this, &DatabaseRequest::sig_MaxMinDate);
    QObject::connect(databaseCache, &DatabaseCache::sig_ScoreboardDay, this, &DatabaseRequest::sig_Scoreboard);
    QObject::connect(databaseCache, &DatabaseCache::sig_AirportChart, this, &DatabaseRequest::sig_ChartWorkload);
}

DatabaseRequest::~DatabaseRequest() {

    delete mutex;
}

void DatabaseRequest::selectListAirports() {

    databaseQueryAirports->selectListAirports();
}

void DatabaseRequest::selectDateByAirport(QString _airport_code) {

    QMutexLocker locker(mutex);

    airport_code = _airport_code;

    if (!databaseCache->selectCalendar(_airport_code)) {

        auto funCalendar = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QString airport = airport_code;

            locker.unlock();

            databaseQueryAirports->selectMaxMinDate(airport);
        });
    }
}

void DatabaseRequest::selectDatabaseSearch(QDate date) {

    QMutexLocker locker(mutex);

    current_date = date;

    if (!databaseCache->selectDatabase(current_date, airport_code)) {

        auto funDay = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QString airport = airport_code;
            QDate date = current_date;
            QDate date_min = min_date;
            QDate date_max = max_date;

            locker.unlock();

            qint32 month_date = date.month();
            qint32 year_date = date.year();

            databaseQueryAirports->selectScoreboardDay(date, airport);

            manyDays(date, date_min, date_max, airport);
        });
    }

    auto funDay = QtConcurrent::run([&] () {

        QThread::msleep(30);

        QMutexLocker locker(mutex);

        QString airport = airport_code;
        QDate date = current_date;
        QDate date_min = min_date;
        QDate date_max = max_date;

        locker.unlock();

        QDate date_variable(date.addMonths(1).year(), date.addMonths(1).month(), 1);

            if (date_variable < max_date) {

            if (!databaseCache->boolManydays(airport, date_variable.toString("yyyy:M"))) {

                manyDays(date_variable, date_min, date_max, airport);
            }
        }

        date_variable = QDate(date.year(), date.month(), 1).addDays(-1);

        if (date_variable > min_date) {

            if (!databaseCache->boolManydays(airport, date_variable.toString("yyyy:M"))) {

                manyDays(date_variable, date_min, date_max, airport);
            }
        }
    });
}

void DatabaseRequest::selectChartWorkload(QDate date) {

    QMutexLocker locker(mutex);

    chart_date = date;

    if (!databaseCache->selectChart(chart_date, airport_code)) {

        auto funManyChar = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QString airport = airport_code;
            QDate date = chart_date;
            QDate date_min = min_date;
            QDate date_max = max_date;

            locker.unlock();

            chartPlusMinus(date, date_min, date_max, airport);
        });
    }

    auto funManyChar = QtConcurrent::run([&] () {

        QThread::msleep(30);

        QMutexLocker locker(mutex);

        QString airport = airport_code;
        QDate date = chart_date;
        QDate date_min = min_date;
        QDate date_max = max_date;

        locker.unlock();

        QDate date_variable = date.addYears(1);

        if (date_variable.year() <= date_max.year()) {

            if (!databaseCache->boolChart(airport_code, date_variable.toString("yyyy"))) {

                chartPlusMinus(date_variable, date_min, date_max, airport);
            }
        }

        date_variable = date.addYears(-1);

        if (date_variable.year() >= date_min.year()) {

            if (!databaseCache->boolChart(airport_code, date_variable.toString("yyyy"))) {

                chartPlusMinus(date_variable, date_min, date_max, airport);
            }
        }
    });
}


void DatabaseRequest::maxMinDate(QString _airport_code, QVector<QDate> max_min) {

    QMutexLocker locker(mutex);

    databaseCache->setAirportCalendar(_airport_code, max_min);

    if (airport_code == _airport_code) {

        max_date = max_min.value(max);
        min_date = max_min.value(min);

        locker.unlock();

        emit sig_MaxMinDate(max_min);
    }
}

void DatabaseRequest::chartWorkload(QString airport, QDate date_scoreboard, QVector<QDate> chart_workload) {

    QMutexLocker locker(mutex);

    databaseCache->setAirportChart(airport, date_scoreboard, chart_workload);

    if (chart_date.year() == date_scoreboard.year()) {

        locker.unlock();

        emit sig_ChartWorkload(date_scoreboard, chart_workload);
    }
}

void DatabaseRequest::manyDays(QDate date_variable, QDate small, QDate big, QString airport) {

    QDate begin_date(date_variable.year(), date_variable.month(), 1);
    QDate end_date = QDate(date_variable.addMonths(1).year(), date_variable.addMonths(1).month(), 1).addDays(-1);

    if (begin_date < small) {

        begin_date = small;
    }

    if (end_date > big) {

        end_date = big.addDays(1);
    }

    databaseQueryAirports->selectScoreboardManydays(begin_date , end_date, airport);
}

void DatabaseRequest::chartPlusMinus(QDate date_variable, QDate small, QDate big, QString airport) {

    QDate begin_date(date_variable.year(), 1, 1);
    QDate end_date(date_variable.year(), 12, 31);

    if (begin_date < small) {

        begin_date = small;
    }

    if (end_date > big) {

        end_date = big;
    }

    databaseQueryAirports->selectChartWorkload(begin_date, end_date, airport);
}
