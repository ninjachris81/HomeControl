#include <QCoreApplication>
#include "controller/controllermanager.h"
#include "appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/errorcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "preheatlogic.h"
#include "heatinglogic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(&appConfig);

    TempController tempController;
    controllerManager.registerController(&tempController);

    ErrorController errorController;
    controllerManager.registerController(&errorController);

    RelayController relayController;
    controllerManager.registerController(&relayController);

    SettingsController settingsController;
    settingsController.setMode(SettingsController::SETTINGS_SERVER);
    controllerManager.registerController(&settingsController);

    controllerManager.init(&appConfig);

    PreheatLogic preheatLogic(&controllerManager);
    HeatingLogic heatLogic(&controllerManager);

    return a.exec();
}
