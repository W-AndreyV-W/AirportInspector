#include "workloadschedule.h"
#include "ui_workloadschedule.h"

WorkloadSchedule::WorkloadSchedule(QString _airport, QDate _date, QVector<QDate> _workload, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkloadSchedule)
{

    airport = _airport;
    date = _date;
    workload  = _workload;

    ui->setupUi(this);

    graphMonth = new QCPGraph(ui->cp_ChartMonth->xAxis, ui->cp_ChartMonth->yAxis);
    graphYear = new QCPBars(ui->cp_ChartYear->xAxis, ui->cp_ChartYear->yAxis);

    ui->cp_ChartMonth->setInteraction(QCP::iRangeZoom, true);
    ui->cp_ChartMonth->setInteraction(QCP::iRangeDrag, true);
    ui->cp_ChartYear->setInteraction(QCP::iRangeZoom, true);
    ui->cp_ChartYear->setInteraction(QCP::iRangeDrag, true);

    pushButtonFalse();

    printchart(airport, date, workload);

    WorkloadSchedule::exec();
}

WorkloadSchedule::~WorkloadSchedule() {

    delete graphMonth;
    delete graphYear;
    delete ui;
}

void WorkloadSchedule::printchart(QString _airport, QDate _date, QVector<QDate> _workload) {

    airport = _airport;
    date = _date;
    workload  = _workload;

    bool month_bool = true;
    QVector<double> graphic_x(MONTH, MONTH);
    QVector<double> graphic_y(MONTH, 0);
    QString date_year;

    for (qsizetype i = 0; i < workload.size(); i++) {

        graphic_y[workload[i].month() - 1]++;
    }

    for (qsizetype i = 0; i < graphic_y.size(); i++) {

        graphic_x[i] = static_cast<double>(i + 1);

        if (graphic_y[i] > 0) {

            switch_month(i);

            if (month_bool) {

                printmonth(i + 1);

                month_bool = false;
            }
        }
    }

    for(int i = 0; i < ui->cp_ChartYear->graphCount(); i++) {

        ui->cp_ChartYear->graph(i)->data()->clear();
    }

    //ui->lb_Airport->setText(airport + " " +  date.toString("yyyy") + " " + date.toString("MMMM"));

    graphYear->setWidth(9/(double)graphic_x.size());

    ui->cp_ChartYear->addGraph();

    graphYear->setData(graphic_x, graphic_y, true);

    ui->cp_ChartYear->rescaleAxes();
    ui->cp_ChartYear->replot();
}

void WorkloadSchedule::clearGraphic() {

    for(int i = 0; i < ui->cp_ChartYear->graphCount(); i++) {

        ui->cp_ChartYear->graph(i)->data()->clear();
    }

    for(int i = 0; i < ui->cp_ChartMonth->graphCount(); i++) {

        ui->cp_ChartMonth->graph(i)->data()->clear();
    }

}

void WorkloadSchedule::printmonth(qint32 num) {

    QDate date_chart;

    if (num != 12) {

        date_chart = QDate(date.year(), num + 1, 1);
    }
    else {

        date_chart = QDate(date.year() + 1, 1, 1);
    }

    QDate end_date = date_chart.addDays(-1);
    QVector<double> graphic_x(end_date.day(), end_date.day());
    QVector<double> graphic_y(end_date.day(), 0);

    for (qsizetype i = 0; i < workload.size(); i++) {

        if (i < end_date.day()) {

            graphic_x[i] = static_cast<double>(i + 1);
        }

        if (workload[i].month() == end_date.month()) {

            graphic_y[workload[i].day() - 1]++;
        }

    }

    for(int i = 0; i < ui->cp_ChartMonth->graphCount(); i++) {

        ui->cp_ChartMonth->graph(i)->data()->clear();
    }

    ui->cp_ChartMonth->addGraph();

    graphMonth->setData(graphic_x, graphic_y, true);

    ui->cp_ChartMonth->rescaleAxes();
    ui->cp_ChartMonth->replot();

    ui->lb_Airport->setText(airport + " (" + end_date.toString("MMMM") + " " + end_date.toString("yyyy") + ")");
}

void WorkloadSchedule::switch_month(qint32 num) {

    switch(num) {

        case 0:
            ui->pb_January->setEnabled(true);
            break;
        case 1:
            ui->pb_February->setEnabled(true);
            break;
        case 2:
            ui->pb_March->setEnabled(true);
            break;
        case 3:
            ui->pb_April->setEnabled(true);
            break;
        case 4:
            ui->pb_May->setEnabled(true);
            break;
        case 5:
            ui->pb_June->setEnabled(true);
            break;
        case 6:
            ui->pb_July->setEnabled(true);
            break;
        case 7:
            ui->pb_August->setEnabled(true);
            break;
        case 8:
            ui->pb_September->setEnabled(true);
            break;
        case 9:
            ui->pb_October->setEnabled(true);
            break;
        case 10:
            ui->pb_November->setEnabled(true);
            break;
        case 11:
            ui->pb_December->setEnabled(true);
            break;
        default:
            break;
        }
}

void WorkloadSchedule::pushButtonFalse() {

    ui->pb_January->setEnabled(false);
    ui->pb_February->setEnabled(false);
    ui->pb_March->setEnabled(false);
    ui->pb_April->setEnabled(false);
    ui->pb_May->setEnabled(false);
    ui->pb_June->setEnabled(false);
    ui->pb_July->setEnabled(false);
    ui->pb_August->setEnabled(false);
    ui->pb_September->setEnabled(false);
    ui->pb_October->setEnabled(false);
    ui->pb_November->setEnabled(false);
    ui->pb_December->setEnabled(false);
}

void WorkloadSchedule::closeEvent(QCloseEvent *event) {

    pushButtonFalse();
}


void WorkloadSchedule::on_pb_January_clicked() {

    printmonth(1);
}

void WorkloadSchedule::on_pb_February_clicked() {

    printmonth(2);
}

void WorkloadSchedule::on_pb_March_clicked() {

    printmonth(3);
}

void WorkloadSchedule::on_pb_April_clicked() {

    printmonth(4);
}

void WorkloadSchedule::on_pb_May_clicked() {

    printmonth(5);
}

void WorkloadSchedule::on_pb_June_clicked() {

    printmonth(6);
}

void WorkloadSchedule::on_pb_July_clicked() {

    printmonth(7);
}

void WorkloadSchedule::on_pb_August_clicked() {

    printmonth(8);
}

void WorkloadSchedule::on_pb_September_clicked() {

    printmonth(9);
}

void WorkloadSchedule::on_pb_October_clicked() {

    printmonth(10);
}

void WorkloadSchedule::on_pb_November_clicked() {

    printmonth(11);
}

void WorkloadSchedule::on_pb_December_clicked() {

    printmonth(12);
}

void WorkloadSchedule::on_pb_Close_clicked() {

    pushButtonFalse();

    WorkloadSchedule::close();
}

