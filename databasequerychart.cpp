#include "databasequerychart.h"

DatabaseQueryChart::DatabaseQueryChart(QSqlQuery* sqlQuery, QObject* parent)
    : DatabaseQuery{sqlQuery, parent} {

}

DatabaseQueryChart::~DatabaseQueryChart() {

}

void DatabaseQueryChart::setData(QDate _begin_date, QDate _end_date, QString _airport_code) {

    QMutexLocker locker(&mutex);

    airport_code = _airport_code;
    begin_date = _begin_date;
    end_date = _end_date;
}

void DatabaseQueryChart::run() {

    mutex.lock();

    if (airport_code != "") {

        QString airport = "'" + airport_code + "'";
        QString end_select = "'" + end_date.toString("yyyy-MM-dd") + "'";
        QString begin_select = "'" + begin_date.toString("yyyy-MM-dd") + "'";
        QString _airport_code = airport_code;
        QDate _date = begin_date;
        //airport_code = "";

        mutex.unlock();

        QVector<QDate> workload;

        if(sqlQuery->exec(select_chart_workload + airport + select_chart_workload_2
                           + airport + select_chart_workload_3 + begin_select
                           + select_chart_workload_4 + end_select + select_chart_workload_5)) {

            convertArray(sqlQuery, workload);
        }

//        if(sqlQuery->exec(select_scoreboard_month + begin_select + select_scoreboard_month_2
//                           + end_select + select_scoreboard_month_3 + airport
//                           + select_scoreboard_month_4 + airport + select_scoreboard_month_5)) {

//            convertArray(sqlQuery, workload, month);
//        }

        emit sig_ChartWorkload(_airport_code, _date, workload);
    }
    else {

        mutex.unlock();
    }
}

void DatabaseQueryChart::convertArray(QSqlQuery* sqlQuery, QVector<QDate>& workload) {

    qsizetype size = sqlQuery->size();

    workload.resize(size);

    for (qsizetype i = 0; sqlQuery->next(); i++) {

        workload[i] = sqlQuery->value(0).toDate();
    }
}
