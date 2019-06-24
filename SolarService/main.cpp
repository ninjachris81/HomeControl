#include <QCoreApplication>
#include "solarcontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SolarController sc;
    sc.init();

    return a.exec();
}
