#include <QCoreApplication>
#include "controllermanager.h"
#include "appconfiguration.h"
#include "tempcontroller.h"
#include "errorcontroller.h"
#include "relaiscontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(&appConfig);

    TempController heatingController;
    controllerManager.registerController(&heatingController);

    ErrorController errorController;
    controllerManager.registerController(&errorController);

    RelaisController relaisController;
    controllerManager.registerController(&relaisController);

    controllerManager.init(&appConfig);

    return a.exec();
}
