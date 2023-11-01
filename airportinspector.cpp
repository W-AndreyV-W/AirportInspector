#include "airportinspector.h"
#include "./ui_airportinspector.h"

AirportInspector::AirportInspector(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AirportInspector)
{
    ui->setupUi(this);

    databaseConnection = new DatabaseConnection(this);
    databaseRequest = new DatabaseRequest(databaseConnection->getSqlDatabase(), this);
    widgetItem = new QTableWidgetItem();

    ui->tw_Arrival->setColumnCount(SETCOLUMN);
    ui->tw_Arrival->setColumnWidth(0, 90);
    ui->tw_Arrival->setColumnWidth(1, 100);
    ui->tw_Arrival->setColumnWidth(2, 150);

    ui->tw_Departure->setColumnCount(SETCOLUMN);
    ui->tw_Departure->setColumnWidth(0, 90);
    ui->tw_Departure->setColumnWidth(1, 100);
    ui->tw_Departure->setColumnWidth(2, 150);

    ui->tw_AirportScoreboard->setEnabled(false);
    ui->lw_Airport->setEnabled(false);
    ui->calendarWidget->setEnabled(false);
    ui->pb_AirportCongestion->setEnabled(false);
    ui->calendarWidget->setSelectedDate(QDate::currentDate());

    QObject::connect(databaseConnection, &DatabaseConnection::sig_SendStatusConnection, this, &AirportInspector::databaseConnectionStatusDay);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_SetListAirports, this, &AirportInspector::printListAirports);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_MaxMinDate, this, &AirportInspector::maxMinDate);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_Scoreboard, this, &AirportInspector::Scoreboard);
    QObject::connect(databaseRequest, &DatabaseRequest::sig_ChartWorkload, this, &AirportInspector::ChartWorkload);
}

AirportInspector::~AirportInspector() {

    delete waorkloadSchedule;
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

    if (max_min.value(max) < ui->calendarWidget->selectedDate()) {

        ui->calendarWidget->setSelectedDate(max_min.value(max));
    }
    else if (max_min.value(min) > ui->calendarWidget->selectedDate()) {

        ui->calendarWidget->setSelectedDate(max_min.value(max));
    }

    ui->calendarWidget->setDateRange(max_min.value(min), max_min.value(max));

    ui->calendarWidget->setGridVisible(true);

    date = ui->calendarWidget->selectedDate();

    databaseRequest->selectDatabaseSearch(date);






    ui->textBrowser->append(max_min.value(max).toString("dd:MM:yyyy"));
    ui->textBrowser->append(max_min.value(min).toString("dd:MM:yyyy"));
}

void AirportInspector::Scoreboard(QVector<QVector<QVector<QString>>> scoreboard) {

    for (qsizetype i = 0; i <= ui->tw_Arrival->rowCount(); i++) {

        ui->tw_Arrival->removeRow(i);
    }

    for (qsizetype i = 0; i <= ui->tw_Departure->rowCount(); i++) {

        ui->tw_Departure->removeRow(i);
    }

    for (qsizetype i = 0; i < scoreboard[arrival].size(); i++) {

        ui->tw_Arrival->setRowCount(scoreboard[arrival].size());

        for (qsizetype j = 0; j < SETCOLUMN; j++) {

            widgetItem->setText(scoreboard[arrival][i][j]);
            ui->tw_Arrival->setItem(i, j, widgetItem->clone());
        }
    }

    for (qsizetype i = 0; i < scoreboard[departure].size(); i++) {

        ui->tw_Departure->setRowCount(scoreboard[departure].size());

        for (qsizetype j = 0; j < SETCOLUMN; j++) {

            widgetItem->setText(scoreboard[departure][i][j]);
            ui->tw_Departure->setItem(i, j, widgetItem->clone());
        }
    }
}

void AirportInspector::ChartWorkload(QVector<QDate> chart_workload) {

    if(waorkloadSchedule == nullptr) {

        waorkloadSchedule = new WorkloadSchedule(this);
    }
    else {

        waorkloadSchedule->clearGraphic();
    }

    waorkloadSchedule->printchart(airport, date, chart_workload);

    waorkloadSchedule->show();
}

void AirportInspector::on_calendarWidget_clicked(const QDate &date) {

    ui->textBrowser->append(date.toString("dd:MM:yyyy"));

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

void AirportInspector::closeEvent(QCloseEvent *event) {

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Close",
                                                               tr("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);

    if (resBtn != QMessageBox::Yes) {

        event->ignore();
    }
    else {

        event->accept();
    }
}


void AirportInspector::on_pb_AirportCongestion_clicked() {

    databaseRequest->selectChartWorkload();
}


void AirportInspector::on_lw_Airport_clicked(const QModelIndex &index) {

    databaseRequest->selectDateByAirport(airport_code[ui->lw_Airport->currentRow()]);

    airport = airport_name[ui->lw_Airport->currentRow()];
}

