#include "solarlogic.h"

SolarLogic::SolarLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, SOLAR_LOGIC_INTERVAL, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));
    m_brightnessController = static_cast<BrightnessController*>(controllerManager->getController(BrightnessController::CONTROLLER_NAME));
}

void SolarLogic::onMaintenance() {
    bool isValid = m_brightnessController->valueIsValid(EnumsDeclarations::BRIGHTNESSES_SOLAR);

    if (isValid) {

    }

}

void SolarLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
