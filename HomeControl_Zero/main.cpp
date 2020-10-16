#include <QCoreApplication>
#include "controller/controllermanager.h"
#include "utils/appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/logcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/brightnesscontroller.h"
#include "controller/switchcontroller.h"
#include "controller/pvcontroller.h"
#include "controller/humiditycontroller.h"
#include "controller/currentcontroller.h"

#include "zerologic.h"
#include "motiondetectorlogic.h"

#include "utils/version.h"

int main(int argc, char *argv[])
{
    qDebug() << "Version" << Version::getVersion();

    QCoreApplication a(argc, argv);

    AppConfiguration appConfig;

    ControllerManager controllerManager(DEV_ID_ZERO, &appConfig);

    /*
    PvController pvController;
    controllerManager.registerController(&pvController);
    */

    HumidityController humidityController;
    controllerManager.registerController(&humidityController);

    LogController logController;
    controllerManager.registerController(&logController);

    RelayController relayController;
    controllerManager.registerController(&relayController);

    SettingsController settingsController;
    controllerManager.registerController(&settingsController);

    TempController tempController;          // needed for settingscontroller
    controllerManager.registerController(&tempController);

    BrightnessController brightnessController;
    controllerManager.registerController(&brightnessController);

    SwitchController switchController;
    controllerManager.registerController(&switchController);

    CurrentController currentController;
    controllerManager.registerController(&currentController);

    controllerManager.init(&appConfig);

    ZeroLogic zeroLogic(&controllerManager);
    MotionDetectorLogic motionDetectorLogic(&controllerManager);

    QObject::connect(&controllerManager, &ControllerManager::mqttConnected, [&logController]() {
        logController.addLog(EnumsDeclarations::LOGS_TYPE_STARTUP, DEV_ID_ZERO);
      });

    return a.exec();
}
