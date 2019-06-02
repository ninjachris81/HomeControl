#include "heatinglogic.h"
#include <QDateTime>

HeatingLogic::HeatingLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, HEATING_LOGIC_INTERVAL, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));
}

void HeatingLogic::startMaintenance() {
    // reset to automatic by default
    m_settingsController->setValue(EnumsDeclarations::SETTINGS_HEATING_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC, true);
    LogicController::startMaintenance();
}

void HeatingLogic::onMaintenance() {
    int heatingMode = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MODE).toInt();
    bool heatingPumpOn = false;

    switch(heatingMode) {
    case EnumsDeclarations::SETTING_MODE_AUTOMATIC:
        if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_INSIDE)) {
            if (isValidMonth()) {
                float heatingTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_TEMP).toDouble();
                float insideTemp = m_tempController->value(EnumsDeclarations::TEMPS_INSIDE).toFloat();
                bool useToggle = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE).toBool();

                if (useToggle) {
                    if (insideTemp<heatingTemp) {

                        if (QDateTime::currentMSecsSinceEpoch()> m_lastHeatOff + HEATING_TOGGLE_ON_DURATION) {
                            // switch off
                            m_lastHeatOff = QDateTime::currentMSecsSinceEpoch();
                        } else {
                            heatingPumpOn = true;
                        }

                    } else {
                        m_lastHeatOff = 0;
                    }
                } else {
                    heatingPumpOn = insideTemp<heatingTemp;
                }
            } else {
                qWarning() << "Out of valid month";
            }
        } else {
            qWarning() << "Inside temp is not valid";
        }
        break;
    case EnumsDeclarations::SETTING_MODE_MANUAL:
        heatingPumpOn = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE).toBool();
        break;
    }

    m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, heatingPumpOn, true);
}

bool HeatingLogic::isValidMonth() {
    int from = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MONTH_FROM).toInt();
    int to = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MONTH_TO).toInt();

    int currentMonth = QDate::currentDate().month();
    if (to<from) {
        to+=12;
        currentMonth+=12;
    }

    return currentMonth>=from && currentMonth<=to;
}

void HeatingLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
