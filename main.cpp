#include "airportinspector.h"

#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    AirportInspector w;

    w.show();

    return a.exec();
}
