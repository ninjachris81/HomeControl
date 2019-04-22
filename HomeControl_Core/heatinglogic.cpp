#include "heatinglogic.h"

HeatingLogic::HeatingLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::staticMetaObject.className()));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::staticMetaObject.className()));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::staticMetaObject.className()));

    startMaintenance(5000);
}

void HeatingLogic::onMaintenance() {
    int from = m_settingsController->value(MQTT_PATH_SETTINGS_PREHEAT_FROM).toInt();
    int to = m_settingsController->value(MQTT_PATH_SETTINGS_PREHEAT_TO).toInt();


    //m_tempController->value(TEMPS_HC)
}

void HeatingLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    switch(cmd) {
    case EnumsDeclarations::START_PREHEAT:
        break;
    }
}
