#ifndef WORKLOADSCHEDULE_H
#define WORKLOADSCHEDULE_H

#include <QDialog>

namespace Ui {
class WorkloadSchedule;
}

class WorkloadSchedule : public QDialog {

    Q_OBJECT

public:

    explicit WorkloadSchedule(QWidget *parent = nullptr);
    ~WorkloadSchedule();

private:

    Ui::WorkloadSchedule *ui;
};

#endif // WORKLOADSCHEDULE_H
