#include "databasequerychart.h"

DatabaseQueryChart::DatabaseQueryChart(QSqlQuery* _sqlQuery, QMutex* _mutex, QObject* parent)
    : QObject{parent} {

    sqlQuery = _sqlQuery;
    mutex = _mutex;
}

DatabaseQueryChart::~DatabaseQueryChart() {

}

void DatabaseQueryChart::selectChartWorkload(QDate begin_date, QDate end_date, QString airport_code) {

    QMutexLocker locker(mutex);

    QString airport_select = "'" + airport_code + "'";
    QString end_select = "'" + end_date.toString("yyyy-MM-dd") + "'";
    QString begin_select = "'" + begin_date.toString("yyyy-MM-dd") + "'";
    QString select = select_chart_workload + airport_select + select_chart_workload_2
                     + airport_select + select_chart_workload_3 + begin_select
                     + select_chart_workload_4 + end_select + select_chart_workload_5;
    QDate date = begin_date;

    if (airport_code != "") {

        QVector<QDate> workload;

        if(sqlQuery->exec(select)) {

            convertArray(workload);
        }

        emit sig_ChartWorkload(airport_code, date, workload);
    }
}

void DatabaseQueryChart::convertArray(QVector<QDate>& workload) {

    qsizetype size = sqlQuery->size();

    workload.resize(size);

    for (qsizetype i = 0; sqlQuery->next(); i++) {

        workload[i] = sqlQuery->value(0).toDate();
    }
}
