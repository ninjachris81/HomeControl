#include "boilerlogic.h"

Q_LOGGING_CATEGORY(LG_BOILER_LOGIC, "BoilerLogic");

BoilerLogic::BoilerLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, BOILER_LOGIC_INTERVAL, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));
}

void BoilerLogic::onMaintenance() {
    bool boilerOn = false;


    m_relayController->setValue(EnumsDeclarations::RELAYS_BOILER, boilerOn, true);
}

void BoilerLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_BOILER_LOGIC) << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
