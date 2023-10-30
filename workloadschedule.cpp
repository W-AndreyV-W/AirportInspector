#include "workloadschedule.h"
#include "ui_workloadschedule.h"

WorkloadSchedule::WorkloadSchedule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkloadSchedule)
{
    ui->setupUi(this);
}

WorkloadSchedule::~WorkloadSchedule()
{
    delete ui;
}
