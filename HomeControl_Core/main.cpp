#include <QCoreApplication>
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

#include "preheatlogic.h"
#include "heatinglogic.h"
#include "boilerlogic.h"
#include "powerlogic.h"
#include "thingspeaklogger.h"
#include "datastatisticsanalyzerlogic.h"

#include "utils/version.h"

int main(int argc, char *argv[])
{
    qDebug() << "Version" << Version::getVersion();

    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(DEV_ID_SERVER, &appConfig);

    WeatherForecastManager::instance()->setApiKey(appConfig.getString("WFC_API_KEY", ""));

    TempController tempController;
    controllerManager.registerController(&tempController);

    LogController logController;
    controllerManager.registerController(&logController);

    RelayController relayController;
    controllerManager.registerController(&relayController);

    SettingsController settingsController;
    controllerManager.registerController(&settingsController);

    BrightnessController brightnessController;
    controllerManager.registerController(&brightnessController);

    SwitchController switchController;
    controllerManager.registerController(&switchController);

    InfoController infoController;
    controllerManager.registerController(&infoController);

    DataLoggerController dataLoggerController;
    controllerManager.registerController(&dataLoggerController);

    PvController pvController;
    controllerManager.registerController(&pvController);

    HumidityController humidityController;
    controllerManager.registerController(&humidityController);

    CurrentController currentController;
    controllerManager.registerController(&currentController);

    PowerController powerController;
    controllerManager.registerController(&powerController);

    controllerManager.init(&appConfig);

    PreheatLogic preheatLogic(&controllerManager);
    HeatingLogic heatLogic(&controllerManager);
    BoilerLogic boilerLogic(&controllerManager, &appConfig);
    DataStatisticsAnalyzerLogic dataStatisticsAnalyzerLogic(&controllerManager);
    PowerLogic powerLogic(&controllerManager);

    ThingSpeakLogger thingspeakLogger(&controllerManager, &appConfig);

    // Register values for dl
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

    // brightnesses
    dataLoggerController.registerValue(&brightnessController, EnumsDeclarations::BRIGHTNESSES_SOLAR);

    // humidities
    dataLoggerController.registerValue(&humidityController, EnumsDeclarations::HUMIDITIES_OUTSIDE);

    // currents
    dataLoggerController.registerValue(&currentController, EnumsDeclarations::CURRENTS_MAIN_BASEMENT);

    // powers
    dataLoggerController.registerValue(&powerController, EnumsDeclarations::POWERS_MAIN);

    QObject::connect(&controllerManager, &ControllerManager::mqttConnected, [&logController]() {
        logController.addLog(EnumsDeclarations::LOGS_TYPE_STARTUP, DEV_ID_SERVER);
      });

    return a.exec();
}
