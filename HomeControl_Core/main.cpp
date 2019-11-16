#include <QCoreApplication>
#include "controller/controllermanager.h"
#include "appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/logcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/brightnesscontroller.h"
#include "controller/switchcontroller.h"
#include "controller/infocontroller.h"

#include "preheatlogic.h"
#include "heatinglogic.h"
#include "thingspeaklogger.h"
#include "datalogger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(true, &appConfig);

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

    BrightnessController brightnessController;
    brightnessController.setMode(ControllerBase::VALUE_OWNER_SERVER);
    controllerManager.registerController(&brightnessController);

    SwitchController switchController;
    switchController.setMode(ControllerBase::VALUE_OWNER_SERVER);
    controllerManager.registerController(&switchController);

    InfoController infoController;
    infoController.setMode(ControllerBase::VALUE_OWNER_SERVER);
    controllerManager.registerController(&infoController);

    controllerManager.init(&appConfig);

    PreheatLogic preheatLogic(&controllerManager);
    HeatingLogic heatLogic(&controllerManager);
    ThingSpeakLogger thingspeakLogger(&controllerManager, &appConfig);
    DataLogger dataLogger(&controllerManager, &appConfig);

    // Register values for dl
    dataLogger.registerValue(&tempController, EnumsDeclarations::TEMPS_HC);
    dataLogger.registerValue(&tempController, EnumsDeclarations::TEMPS_TANK);
    dataLogger.registerValue(&tempController, EnumsDeclarations::TEMPS_WATER);
    dataLogger.registerValue(&tempController, EnumsDeclarations::TEMPS_SOLAR_HC);
    dataLogger.registerValue(&tempController, EnumsDeclarations::TEMPS_INSIDE);

    QObject::connect(&controllerManager, &ControllerManager::mqttConnected, [&logController]() {
        logController.addLog(EnumsDeclarations::LOGS_TYPE_STARTUP, DEV_ID_SERVER);
      });

    return a.exec();
}
