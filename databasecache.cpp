#include "databasecache.h"

DatabaseCache::DatabaseCache(QObject *parent) : QObject{parent} {

}

DatabaseCache::~DatabaseCache() {

}

void DatabaseCache::setAirportCalendar(QString _airport_code, QVector<QDate> _airport_calendar) {

    mutex.lock();

    airport_calendar_set = _airport_calendar;
    airport_code_set_calendar = _airport_code;

    mutex.unlock();

    auto func = QtConcurrent::run([&] () {

        QMutexLocker locker(&mutex);

        airport_code.insert(airport_code_set_calendar, QDateTime::currentDateTime());
        airport_calendar.insert(airport_code_set_calendar, airport_calendar_set);
    });
}

void DatabaseCache::setAirportDatabase(QString _airport_code, QDate date, QVector<QVector<QVector<QString>>> _airport_scoreboard) {

    mutex.lock();

    airport_scoreboard_set = _airport_scoreboard;
    airport_code_set_scoreboard = _airport_code;
    date_set_scoreboard = date;

    mutex.unlock();

    auto func = QtConcurrent::run([&] () {

        QDateTime time = QDateTime::currentDateTime();

        QMutexLocker locker(&mutex);

        QMap<QString, QDateTime> base;

        if (airport_database.contains(airport_code_set_scoreboard)) {

            airport_database[airport_code_set_scoreboard].insert(date_set_scoreboard.toString("yyyy:M"), time);
        }
        else {

            base.insert(date_set_scoreboard.toString("yyyy:M"), time);
            airport_database.value(airport_code_set_scoreboard, base);
        }


        database_cache.insert(time, airport_scoreboard_set);
        time_airport_code.insert(time, airport_code_set_scoreboard);


        while (database_cache.size() > CACHE_SIZE) {

            QDateTime delete_time = database_cache.firstKey();
            QString code_airport = time_airport_code.value(delete_time);
            QString name_date = airport_database[code_airport].key(delete_time);

            database_cache.remove(delete_time);
            time_airport_code.remove(delete_time);
            airport_database[code_airport].remove(name_date);

            if (airport_database[code_airport].isEmpty()) {

                airport_database.remove(code_airport);
                airport_calendar.remove(code_airport);
                airport_code.remove(code_airport);
            }
        }
    });
}

bool DatabaseCache::selectDatabase(QDate _date, QString _airport_code) {

    mutex.lock();

    select_date = _date;
    select_airport_code = _airport_code;

    QMap<QString, QDateTime> base = airport_database.value(select_airport_code);

    if (base.contains(select_date.toString("yyyy:M"))) {

        mutex.unlock();

        auto finc = QtConcurrent::run([&] () {

            QDateTime time = QDateTime::currentDateTime();

            QMutexLocker locker(&mutex);

            QDateTime time_database = airport_database[select_airport_code].value(select_date.toString("yyyy:M"));
            QVector<QVector<QVector<QString>>> day_scoreboard(2);

            selectVector(database_cache, day_scoreboard, time_database, arrival_c);
            selectVector(database_cache, day_scoreboard, time_database, departure_c);

            locker.unlock();

            emit sig_ScoreboardDay(day_scoreboard);

            locker.relock();

            QVector<QVector<QVector<QString>>> database= database_cache.value(time_database);
            QString code = time_airport_code.value(time_database);


            airport_database[select_airport_code].insert(select_date.toString("yyyy:M"), time);
            database_cache.insert(time, database);
            time_airport_code.insert(time, code);
            database_cache.remove(time_database);
            time_airport_code.remove(time_database);
        });

        return true;
    }
    else {

        mutex.unlock();

        return false;
    }
}

bool DatabaseCache::selectCalendar(QString _airport_code) {

    //return false;

    if (airport_calendar.contains(_airport_code)) {

        emit sig_MaxMinDate(airport_calendar.value(_airport_code));

        return true;
    }
    else {


        return false;
    }
}

bool DatabaseCache::boolManydays(QString _airport_code, QString date_manydays) {

    if (airport_database[_airport_code].contains(date_manydays)) {

        return true;
    }

    return false;
}

void DatabaseCache::selectVector(QMap<QDateTime, QVector<QVector<QVector<QString>>>>& cache,
                                 QVector<QVector<QVector<QString>>>& day, QDateTime time_db, qint8 index) {

    for (qsizetype i = 0, j = 0; i < cache[time_db].value(index).size(); i++) {

        if (cache[time_db][index][i].value(scheduled_c) == select_date.toString("dd:MM:yyyy")){

            QVector<QString> db(SET_COLUMN_DAY);

            db[flight_no_c] = cache[time_db][index][i].value(flight_no_c);
            db[scheduled_departure_c] = cache[time_db][index][i].value(scheduled_departure_c);
            db[airport_name_c] = cache[time_db][index][i].value(airport_name_c);
            day[index].insert(j, db);

            j++;
        }
    }
}
