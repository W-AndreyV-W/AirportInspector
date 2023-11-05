#include "databasecache.h"

DatabaseCache::DatabaseCache(QObject *parent)
    : QObject{parent} {

    mutex = new QMutex();
}

DatabaseCache::~DatabaseCache() {

    delete mutex;

}

void DatabaseCache::setAirportCalendar(QString _airport_code, QVector<QDate> _airport_calendar) {

    QMutexLocker locker(mutex);

    airport_data_set = _airport_calendar;
    airport_max_min = _airport_code;

    auto func = QtConcurrent::run([&] () {

        QMutexLocker locker(mutex);

        QDateTime time = QDateTime::currentDateTime();

        //  Сохранение в Cash
        airport_code_calendar.insert(airport_max_min, time);
        airport_calendar.insert(time, airport_data_set);

        //  Ограничение размера Cash
        while (airport_code_calendar.size() > CACHE_SIZE / 4) {

            QDateTime delete_time = airport_calendar.firstKey();
            QString code_airport = airport_code_calendar.key(delete_time);

            // Удаление из Cash
            airport_calendar.remove(delete_time);
            airport_code_calendar.remove(code_airport);
        }
    });
}

void DatabaseCache::setAirportChart(QString _airport_code, QDate date, QVector<QDate> chart_workload) {

    QMutexLocker locker(mutex);

    chart_data_set = chart_workload;
    date_chart = date;
    airport_chart_set = _airport_code;

    auto func = QtConcurrent::run([&] () {

        QMutexLocker locker(mutex);

        QDateTime time = QDateTime::currentDateTime();

        //  Сохранение в Cash
        if (airport_code_chart.contains(airport_chart_set)) {

            airport_code_chart[airport_chart_set].insert(date_chart.toString("yyyy"), time);
        }
        else {

            QMap<QString, QDateTime> base;
            base.insert(date_chart.toString("yyyy"), time);
            airport_code_chart.insert(airport_chart_set, base);
        }

        airport_chart.insert(time, chart_data_set);
        time_airport_chart.insert(time, airport_chart_set);

        //  Ограничение размера Cash
        while (time_airport_chart.size() > CACHE_SIZE / 2) {

            QDateTime delete_time = time_airport_chart.firstKey();
            QString code_airport = time_airport_chart.value(delete_time);
            QString name_date = airport_code_chart[code_airport].key(delete_time);

            // Удаление из Cash
            airport_chart.remove(delete_time);
            time_airport_chart.remove(delete_time);
            airport_code_chart[code_airport].remove(name_date);

            if (airport_code_chart[code_airport].isEmpty()) {

                airport_code_chart.remove(code_airport);
            }
        }
    });
}

void DatabaseCache::setAirportDatabase(QString _airport_code, QDate date, QVector<QVector<QVector<QString>>> _airport_scoreboard) {

    QMutexLocker locker(mutex);

    airport_scoreboard_set = _airport_scoreboard;
    airport_scoreboard = _airport_code;
    date_scoreboard = date;

    auto func = QtConcurrent::run([&] () {

        QMutexLocker locker(mutex);

        QDateTime time = QDateTime::currentDateTime();

        //  Сохранение в Cash
        if (airport_database.contains(airport_scoreboard)) {

            airport_database[airport_scoreboard].insert(date_scoreboard.toString("yyyy:M"), time);
        }
        else {

            QMap<QString, QDateTime> base;
            base.insert(date_scoreboard.toString("yyyy:M"), time);
            airport_database.value(airport_scoreboard, base);
        }

        database_cache.insert(time, airport_scoreboard_set);
        time_airport_database.insert(time, airport_scoreboard);

        //  Ограничение размера Cash
        while (time_airport_database.size() > CACHE_SIZE) {

            QDateTime delete_time = time_airport_database.firstKey();
            QString code_airport = time_airport_database.value(delete_time);
            QString name_date = airport_database[code_airport].key(delete_time);

            // Удаление из Cash
            database_cache.remove(delete_time);
            time_airport_database.remove(delete_time);
            airport_database[code_airport].remove(name_date);

            if (airport_database[code_airport].isEmpty()) {

                airport_database.remove(code_airport);
            }
        }
    });
}

bool DatabaseCache::selectDatabase(QDate _date, QString _airport_code) {

    QMutexLocker locker(mutex);

    select_date = _date;
    select_airport_code = _airport_code;

    QMap<QString, QDateTime> base = airport_database.value(select_airport_code);

    if (base.contains(select_date.toString("yyyy:M"))) {

        auto finc = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QDateTime time = QDateTime::currentDateTime();

            QDateTime time_database = airport_database[select_airport_code].value(select_date.toString("yyyy:M"));
            QVector<QVector<QVector<QString>>> day_scoreboard(2);

            selectVector(database_cache, day_scoreboard, time_database, arrival_c);
            selectVector(database_cache, day_scoreboard, time_database, departure_c);

            // Запись в Cash о поиске по аэропорту
            QVector<QVector<QVector<QString>>> database = database_cache.value(time_database);

            airport_database[select_airport_code].insert(select_date.toString("yyyy:M"), time);
            database_cache.insert(time, database);
            time_airport_database.insert(time, select_airport_code);
            database_cache.remove(time_database);
            time_airport_database.remove(time_database);

            emit sig_ScoreboardDay(day_scoreboard);
        });

        return true;
    }
    else {

        return false;
    }
}

bool DatabaseCache::selectCalendar(QString _airport_code) {

    QMutexLocker locker(mutex);

    select_airport_code = _airport_code;

    if (airport_code_calendar.contains(select_airport_code)) {

        auto finc = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QDateTime time = QDateTime::currentDateTime();

            QDateTime time_calendar = airport_code_calendar.value(select_airport_code);

            // Запись в Cash о поиске по аэропорту
            QVector<QDate> database = airport_calendar.value(time_calendar);

            airport_code_calendar.insert(select_airport_code, time);
            airport_calendar.insert(time, database);
            airport_calendar.remove(time_calendar);

            emit sig_MaxMinDate(database);
        });

        return true;
    }
    else {

        return false;
    }
}

bool DatabaseCache::selectChart(QDate date, QString _airport_code) {

    QMutexLocker locker(mutex);

    select_date = date;
    select_airport_code = _airport_code;

    QMap<QString, QDateTime> base = airport_code_chart.value(select_airport_code);

    if (base.contains(select_date.toString("yyyy"))) {

        auto finc = QtConcurrent::run([&] () {

            QMutexLocker locker(mutex);

            QDateTime time = QDateTime::currentDateTime();

            QDateTime time_chart = airport_code_chart[select_airport_code].value(select_date.toString("yyyy"));

            QVector<QDate> database = airport_chart.value(time_chart);

            // Запись в Cash о поиске по аэропорту
            airport_code_chart[select_airport_code].insert(select_date.toString("yyyy"), time);
            airport_chart.insert(time, database);
            time_airport_chart.insert(time, select_airport_code);
            airport_chart.remove(time_chart);
            time_airport_chart.remove(time_chart);

            emit sig_AirportChart(select_date, database);
        });

        return true;
    }
    else {

        return false;
    }
}

bool DatabaseCache::boolManydays(QString _airport_code, QString date_txt) {

    QMutexLocker locker(mutex);

    if (airport_database[_airport_code].contains(date_txt)) {

        return true;
    }
    else {

        return false;
    }
}

bool DatabaseCache::boolChart(QString _airport_code, QString date_txt) {

    QMutexLocker locker(mutex);

    if (airport_code_chart[_airport_code].contains(date_txt)) {

        return true;
    }
    else {

        return false;
    }
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
