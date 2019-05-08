#include <QCoreApplication>
#include "controller/controllermanager.h"
#include "appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/logcontroller.h"
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

    LogController logController;
    logController.setMode(ControllerBase::VALUE_OWNER_SERVER);
    controllerManager.registerController(&logController);

    RelayController relayController;
    controllerManager.registerController(&relayController);

    SettingsController settingsController;
    settingsController.setMode(ControllerBase::VALUE_OWNER_SERVER);
    controllerManager.registerController(&settingsController);

    controllerManager.init(&appConfig);

    PreheatLogic preheatLogic(&controllerManager);
    HeatingLogic heatLogic(&controllerManager);

    return a.exec();
}
