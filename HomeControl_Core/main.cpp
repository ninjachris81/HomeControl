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

    TempController heatingController(&appConfig);
    controllerManager.registerController(&heatingController);

    ErrorController errorController(&appConfig);
    controllerManager.registerController(&errorController);

    RelaisController relaisController(&appConfig);
    controllerManager.registerController(&relaisController);

    controllerManager.init();

    return a.exec();
}
