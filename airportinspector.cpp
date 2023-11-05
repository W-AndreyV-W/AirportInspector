#include "airportinspector.h"
#include "./ui_airportinspector.h"

AirportInspector::AirportInspector(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AirportInspector)
{
    ui->setupUi(this);

    databaseConnection = new DatabaseConnection(this);
    databaseRequest = new DatabaseRequest(&databaseConnection->getSqlDatabase(), this);
    widgetItem = new QTableWidgetItem();

    ui->tw_Arrival->setColumnWidth(0, 90);
    ui->tw_Arrival->setColumnWidth(1, 100);
    ui->tw_Arrival->setColumnWidth(2, 150);

    ui->tw_Departure->setColumnWidth(0, 90);
    ui->tw_Departure->setColumnWidth(1, 100);
    ui->tw_Departure->setColumnWidth(2, 150);

    ui->tw_AirportScoreboard->setEnabled(false);
    ui->lw_Airport->setEnabled(false);
    ui->calendarWidget->setEnabled(false);
    ui->pb_AirportCongestion->setEnabled(false);
    ui->lb_Workload->setEnabled(false);
    ui->calendarWidget->setSelectedDate(QDate::currentDate());

    ui->lb_Workload->setText("Загруженности аэропорта 0 в/п в день");

    QObject::connect(databaseConnection, &DatabaseConnection::sig_SendStatusConnection, this, &AirportInspector::databaseConnectionStatusDay);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_SetListAirports, this, &AirportInspector::printListAirports);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_MaxMinDate, this, &AirportInspector::maxMinDate);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_Scoreboard, this, &AirportInspector::Scoreboard);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_ChartWorkload, this, &AirportInspector::chartWorkload);
}

AirportInspector::~AirportInspector() {

    delete workloadSchedule;
    delete widgetItem;
    delete ui;
}

void AirportInspector::databaseConnectionStatusDay(bool status) {

    if (status) {

        databaseRequest->selectListAirports();

        ui->lb_statusConnection->setText("Подключено");
        ui->tw_AirportScoreboard->setEnabled(true);
    }
    else {

        databaseConnectionError(*databaseConnection);
    }
}

void AirportInspector::printListAirports(QVector<QString> _airport_code, QVector<QString> _list_airports) {

    for (qsizetype i = 0; i < _airport_code.size(); i++) {

        ui->lw_Airport->addItem(_airport_code[i] + "\t" + _list_airports[i]);
    }

    ui->lw_Airport->setEnabled(true);

    airport_code = _airport_code;
    airport_name = _list_airports;
}

void AirportInspector::maxMinDate(QVector<QDate> max_min) {

    ui->calendarWidget->setEnabled(true);
    ui->pb_AirportCongestion->setEnabled(true);
    ui->lb_Workload->setEnabled(true);

    date_max = max_min.value(max);
    date_min = max_min.value(min);

    if (date_max < ui->calendarWidget->selectedDate()) {

        ui->calendarWidget->setSelectedDate(date_max);
    }
    else if (date_min > ui->calendarWidget->selectedDate()) {

        ui->calendarWidget->setSelectedDate(date_min);
    }

    ui->calendarWidget->setDateRange(date_min, date_max);

    ui->calendarWidget->setGridVisible(true);

    date_airport = ui->calendarWidget->selectedDate();

    databaseRequest->selectDatabaseSearch(date_airport);
}

void AirportInspector::Scoreboard(QVector<QVector<QVector<QString>>> scoreboard) {

    qint32 workload = scoreboard[arrival_c].size() + scoreboard[departure_c].size();

    ui->lb_Workload->setText("Загруженности аэропорта " + QString::number(workload) + " в/п в день");

    for (qsizetype i = 0; i <= ui->tw_Arrival->rowCount(); i++) {

        ui->tw_Arrival->removeRow(i);
    }

    for (qsizetype i = 0; i <= ui->tw_Departure->rowCount(); i++) {

        ui->tw_Departure->removeRow(i);
    }

    for (qsizetype i = 0; i < scoreboard[arrival_c].size(); i++) {

        ui->tw_Arrival->setRowCount(scoreboard[arrival_c].size());

        for (qsizetype j = 0; j < SETCOLUMN; j++) {

            widgetItem->setText(scoreboard[arrival_c][i][j]);
            ui->tw_Arrival->setItem(i, j, widgetItem->clone());
        }
    }

    for (qsizetype i = 0; i < scoreboard[departure_c].size(); i++) {

        ui->tw_Departure->setRowCount(scoreboard[departure_c].size());

        for (qsizetype j = 0; j < SETCOLUMN; j++) {

            widgetItem->setText(scoreboard[departure_c][i][j]);
            ui->tw_Departure->setItem(i, j, widgetItem->clone());
        }
    }
}

void AirportInspector::selectChartWorkload(QDate date) {

    databaseRequest->selectChartWorkload(date);
}

void AirportInspector::chartWorkload(QDate date, QVector<QDate> chart_workload) {

    if(workloadSchedule == nullptr) {

        workloadSchedule = new WorkloadSchedule(airport, date, date_max, date_min, chart_workload, this);

        QObject::connect(workloadSchedule, &WorkloadSchedule::sig_selectChartWorkload, databaseRequest, &DatabaseRequest::selectChartWorkload);
    }
    else {

        if (workloadSchedule->isActiveWindow()) {

            workloadSchedule->setWorkloadSchedule(airport, date, date_max, date_min, chart_workload);
        }

        workloadSchedule->setWorkloadSchedule(airport, date, date_max, date_min, chart_workload);

        workloadSchedule->show();
    }
}

void AirportInspector::on_calendarWidget_clicked(const QDate &date) {

    date_airport = date;

    databaseRequest->selectDatabaseSearch(date);
}

void AirportInspector::databaseConnectionError(DatabaseConnection& database) {

    QMessageBox::StandardButton msg = QMessageBox::warning(this, "Error", tr("Ошибка подключения к базе данных!"),
                                                                QMessageBox::Retry | QMessageBox::Close, QMessageBox::Close);


    if (msg == QMessageBox::Close) {

        AirportInspector::close();

    }
    else {

        auto fun = QtConcurrent::run([&] () {

            QThread::sleep(5);

            database.connectToDatabase();
        });
    }
}

void AirportInspector::on_pb_AirportCongestion_clicked() {

    databaseRequest->selectChartWorkload(date_airport);
}


void AirportInspector::on_lw_Airport_clicked(const QModelIndex &index) {

    databaseRequest->selectDateByAirport(airport_code[ui->lw_Airport->currentRow()]);

    airport = airport_name[ui->lw_Airport->currentRow()];
}

