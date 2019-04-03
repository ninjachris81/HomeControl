#include <QCoreApplication>
#include "controllermanager.h"
#include "appconfiguration.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(&appConfig);

    controllerManager.init();

    return a.exec();
}
