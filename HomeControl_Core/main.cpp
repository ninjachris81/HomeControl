#include <QCoreApplication>
#include "controllermanager.h"
#include "appconfiguration.h"
#include "tempcontroller.h"
#include "errorcontroller.h"
#include "relaiscontroller.h"
#include "settingscontroller.h"
#include "preheatlogic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(&appConfig);

    TempController tempController;
    controllerManager.registerController(&tempController);

    ErrorController errorController;
    controllerManager.registerController(&errorController);

    RelaisController relaisController;
    controllerManager.registerController(&relaisController);

    SettingsController settingsController(SettingsController::SETTINGS_SERVER);
    controllerManager.registerController(&settingsController);

    controllerManager.init(&appConfig);

    PreheatLogic preheatLogic(&controllerManager);

    return a.exec();
}
