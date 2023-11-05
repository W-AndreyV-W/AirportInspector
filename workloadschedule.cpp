#include "workloadschedule.h"
#include "ui_workloadschedule.h"

WorkloadSchedule::WorkloadSchedule(QString _airport, QDate _date, QDate _date_max, QDate _date_min, QVector<QDate> chart_workload, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkloadSchedule)
{

    airport = _airport;
    date = _date;
    date_max = _date_max;
    date_min = _date_min;

    ui->setupUi(this);

    graphMonth = new QCPGraph(ui->cp_ChartMonth->xAxis, ui->cp_ChartMonth->yAxis);
    graphYear = new QCPBars(ui->cp_ChartYear->xAxis, ui->cp_ChartYear->yAxis);

    ui->lb_Airport->setText(airport);

    printYear(chart_workload);

    WorkloadSchedule::open();
}

WorkloadSchedule::~WorkloadSchedule() {

    delete ui;
}

void WorkloadSchedule::setWorkloadSchedule(QString _airport, QDate _date, QDate _date_max, QDate _date_min, QVector<QDate> chart_workload) {

    airport = _airport;
    date = _date;
    date_max = _date_max;
    date_min = _date_min;

    printYear(chart_workload);
}

void WorkloadSchedule::printYear(QVector<QDate> workload) {

    qint32 year = date_max.year();
    bool month_bool = true;

    workload_month.clear();
    workload_year.clear();;

    workload_month.resize(MONTH);
    workload_year.resize(MONTH, 0);

    for (qsizetype i = 0; i < MONTH; i++) {

        QDate date_graphic(date.year(), i + 1, 1);

        workload_month[i].resize(date_graphic.addMonths(1).addDays(-1).day(), 0);
    }

    for (qsizetype i = 0; i < workload.size(); i++) {

        workload_year[workload[i].month() - 1]++;
        workload_month[(workload[i].month() - 1)][(workload[i].day() - 1)]++;
    }

    QVector<double> graphic_x(MONTH, MONTH);
    QVector<double> graphic_y = workload_year;

    for (qsizetype i = 0; i < MONTH; i++) {

        graphic_x[i] = static_cast<double>(i + 0.5);

        if (graphic_y[i] > 0) {

            if (month_bool) {

                month = i + 1;

                printmonth(month);

                month_bool = false;
            }
        }
    }

    for(int i = 0; i < ui->cp_ChartYear->graphCount(); i++) {

        ui->cp_ChartYear->graph(i)->data()->clear();
    }

    ui->lw_YearUp->clear();
    ui->lw_YearDown->clear();
    ui->lw_MonthYearUp->clear();
    ui->lw_MonthYearDown->clear();

    while (year > date.year()) {

        ui->lw_YearUp->addItem(QString::number(year));
        ui->lw_MonthYearUp->addItem(QString::number(year));

        year--;
    }

    ui->lb_Year->setText(QString::number(year));

    year--;

    while (year >= date_min.year()) {

        ui->lw_YearDown->addItem(QString::number(year));
        ui->lw_MonthYearDown->addItem(QString::number(year));

        year--;
    }

    ui->lb_Year->setText(date.toString("yyyy"));

    graphYear->setData(graphic_x, graphic_y, true);
    graphYear->setWidth(8/(double)graphic_x.size());

    graphYear->rescaleAxes();

    ui->cp_ChartYear->xAxis->setLabel("Месяц");
    ui->cp_ChartYear->yAxis->setLabel("Количество прилетов/вылетов");

    ui->cp_ChartYear->xAxis->grid()->setSubGridVisible(true);

    ui->cp_ChartYear->xAxis->setNumberFormat("f");
    ui->cp_ChartYear->yAxis->setNumberFormat("f");
    ui->cp_ChartYear->xAxis->setNumberPrecision(0);
    ui->cp_ChartYear->yAxis->setNumberPrecision(0);
    ui->cp_ChartYear->xAxis->setRange(0, 12);

    ui->cp_ChartYear->replot();
}

void WorkloadSchedule::printmonth(qint32 month_print) {

    month_up.resize(MONTH);
    month_down.resize(MONTH);

    QVector<double> graphic_x(workload_month[month_print - 1].size());
    QVector<double> graphic_y = workload_month[month_print - 1];

    for (qsizetype i = 0; i < workload_month[month_print - 1].size(); i++) {

        graphic_x[i] = static_cast<double>(i + 1);
    }

    ui->lw_MonthUp->clear();
    ui->lw_MonthDown->clear();


    for (qsizetype i = 0, j = 0; i < month_print - 1; i++) {

        if (workload_year[i] > 0) {

            ui->lw_MonthUp->addItem(switch_month(i));

            month_up[j] = i + 1;

            j++;
        }
    }

    ui->lb_MonthYear->setText(switch_month(month_print - 1) + " " + date.toString("yyyy"));


    for (qsizetype i = month_print, j = 0; i < MONTH; i++) {

        if (workload_year[i] > 0) {

            ui->lw_MonthDown->addItem(switch_month(i));

            month_down[j] = i + 1;

            j++;
        }
    }

    for(int i = 0; i < ui->cp_ChartMonth->graphCount(); i++) {

        ui->cp_ChartMonth->graph(i)->data()->clear();
    }

    graphMonth->setData(graphic_x, graphic_y, true);

    ui->cp_ChartMonth->xAxis->setLabel("Число месяца");
    ui->cp_ChartMonth->yAxis->setLabel("Количество прилетов/вылетов");

    graphMonth->rescaleAxes();

    ui->cp_ChartMonth->xAxis->setNumberFormat("f");
    ui->cp_ChartMonth->yAxis->setNumberFormat("f");
    ui->cp_ChartMonth->xAxis->setNumberPrecision(0);
    ui->cp_ChartMonth->yAxis->setNumberPrecision(0);

    ui->cp_ChartMonth->replot();
}

QString WorkloadSchedule::switch_month(qint32 num) {

    switch(num) {

        case 0:
            return "Январь";
        case 1:
            return "Февраль";
        case 2:
            return "Март";
        case 3:
            return "Апрель";
        case 4:
            return "Май";
        case 5:
            return "Июнь";
        case 6:
            return "Июль";
        case 7:
            return "Август";
        case 8:
            return "Сентябрь";
        case 9:
            return "Октябрь";
        case 10:
            return "Ноябрь";
        case 11:
            return "Декабрь";
        default:
            return "-------";
        }
}

void WorkloadSchedule::on_pb_Close_clicked() {

    WorkloadSchedule::close();
}

void WorkloadSchedule::on_lw_YearUp_itemClicked(QListWidgetItem *item) {

    date = QDate(item->text().toInt(), 7, 1);

    emit sig_selectChartWorkload(date);
}

void WorkloadSchedule::on_lw_YearDown_itemClicked(QListWidgetItem *item) {

    date = QDate(item->text().toInt(), 7, 1);

    emit sig_selectChartWorkload(date);
}

void WorkloadSchedule::on_lw_MonthYearUp_itemClicked(QListWidgetItem *item) {

    date = QDate(item->text().toInt(), 7, 1);

    emit sig_selectChartWorkload(date);
}

void WorkloadSchedule::on_lw_MonthYearDown_itemClicked(QListWidgetItem *item) {

    date = QDate(item->text().toInt(), 7, 1);

    emit sig_selectChartWorkload(date);
}

void WorkloadSchedule::on_lw_MonthUp_clicked(const QModelIndex &index) {

    printmonth(month_up.value(index.row()));
}

void WorkloadSchedule::on_lw_MonthDown_clicked(const QModelIndex &index) {

    printmonth(month_down.value(index.row()));
}

