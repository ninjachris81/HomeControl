#include <QCoreApplication>
#include <QDebug>

#include "controller/controllermanager.h"
#include "utils/appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/logcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/brightnesscontroller.h"
#include "controller/switchcontroller.h"
#include "controller/infocontroller.h"
#include "controller/dataloggercontroller.h"
#include "controller/pvcontroller.h"
#include "controller/humiditycontroller.h"
#include "controller/currentcontroller.h"
#include "controller/powercontroller.h"
#include "controller/eventcontroller.h"

#include "powerlogic.h"
//include "thingspeaklogger.h"
//#include "datastatisticsanalyzerlogic.h"
//#include "systasmartcclient.h"
#include "serialextensionclient.h"
#include "doorbelllogic.h"
#include "httpserver.h"

#include "utils/version.h"

int main(int argc, char *argv[])
{
    qDebug() << "Version" << Version::getVersion();

    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(DEV_ID_SERVER, &appConfig);

    /*
    TempController tempController;
    controllerManager.registerController(&tempController);

    RelayController relayController;
    controllerManager.registerController(&relayController);

    SwitchController switchController;
    controllerManager.registerController(&switchController);

    PvController pvController;
    controllerManager.registerController(&pvController);

    DataStatisticsAnalyzerLogic dataStatisticsAnalyzerLogic(&controllerManager);

    ThingSpeakLogger thingspeakLogger(&controllerManager, &appConfig);

    // temps
    dataLoggerController.registerValue(&tempController, EnumsDeclarations::TEMPS_HC);
    dataLoggerController.registerValue(&tempController, EnumsDeclarations::TEMPS_TANK);
    dataLoggerController.registerValue(&tempController, EnumsDeclarations::TEMPS_WATER);
    dataLoggerController.registerValue(&tempController, EnumsDeclarations::TEMPS_SOLAR_HC);
    dataLoggerController.registerValue(&tempController, EnumsDeclarations::TEMPS_INSIDE);
    dataLoggerController.registerValue(&tempController, EnumsDeclarations::TEMPS_OUTSIDE);

    // relays
    dataLoggerController.registerValue(&relayController, EnumsDeclarations::RELAYS_HC_PUMP);
    dataLoggerController.registerValue(&relayController, EnumsDeclarations::RELAYS_HEATING_PUMP);
    dataLoggerController.registerValue(&relayController, EnumsDeclarations::RELAYS_BOILER);

    */

    DataLoggerController dataLoggerController;
    controllerManager.registerController(&dataLoggerController);

    LogController logController;
    controllerManager.registerController(&logController);

    SettingsController settingsController;
    controllerManager.registerController(&settingsController);

    InfoController infoController;
    controllerManager.registerController(&infoController);

    BrightnessController brightnessController;
    controllerManager.registerController(&brightnessController);

    HumidityController humidityController;
    controllerManager.registerController(&humidityController);

    EventController eventController;
    controllerManager.registerController(&eventController);

    CurrentController currentController;
    controllerManager.registerController(&currentController);

    PowerController powerController;
    controllerManager.registerController(&powerController);

    // Register values for dl
    // brightnesses
    dataLoggerController.registerValue(&brightnessController, EnumsDeclarations::BRIGHTNESSES_SOLAR);

    // humidities
    dataLoggerController.registerValue(&humidityController, EnumsDeclarations::HUMIDITIES_OUTSIDE);

    // currents
    dataLoggerController.registerValue(&currentController, EnumsDeclarations::CURRENTS_MAIN_BASEMENT);

    // powers
    dataLoggerController.registerValue(&powerController, EnumsDeclarations::POWERS_MAIN);

    controllerManager.init(&appConfig);

    //SystaSmartCClient systaSmartCClient(appConfig);

    PowerLogic powerLogic(&controllerManager);

    DoorBellLogic doorBellLogic(&controllerManager);

    SerialExtensionClient serialExtensionClient(&controllerManager, appConfig);

    HttpServer httpServer(&controllerManager, appConfig, &a);

    QObject::connect(&controllerManager, &ControllerManager::mqttConnected, [&logController]() {
        logController.addLog(EnumsDeclarations::LOGS_TYPE_STARTUP, DEV_ID_SERVER);
      });

    return a.exec();
}
