#include "heatinglogic.h"
#include <QDateTime>

HeatingLogic::HeatingLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));

    startMaintenance(5000);
}

void HeatingLogic::onMaintenance() {
    int heatingMode = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MODE).toInt();

    switch(heatingMode) {
    case EnumsDeclarations::SETTING_MODE_AUTOMATIC:
        if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_INSIDE)) {
            float heatingTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_TEMP).toDouble();
            float insideTemp = m_tempController->value(EnumsDeclarations::TEMPS_INSIDE).toFloat();
            bool useToggle = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE).toBool();

            if (useToggle) {
                if (insideTemp<heatingTemp) {

                    if (QDateTime::currentMSecsSinceEpoch()> m_lastHeatOff + HEATING_TOGGLE_ON_DURATION) {
                        // switch off
                        m_lastHeatOff = QDateTime::currentMSecsSinceEpoch();
                        m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, false, true);
                    } else {
                        m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, true, true);
                    }

                } else {
                    m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, false, true);
                    m_lastHeatOff = 0;
                }
            } else {
                m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, insideTemp<heatingTemp, true);
            }

        } else {
            qWarning() << "Inside temp is not valid";
            m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, false, true);
        }
        break;
    case EnumsDeclarations::SETTING_MODE_MANUAL:
        bool heatingState = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE).toBool();
        m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, heatingState, true);
        break;
    }
}

void HeatingLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
