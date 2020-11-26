#include "heatinglogic.h"
#include <QDateTime>

Q_LOGGING_CATEGORY(LG_HEATING_LOGIC, "HeatingLogic");

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
                if (isValidTime()) {
                    if (isValidTankTemp()) {
                        float heatingTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_TEMP).toDouble();
                        float insideTemp = m_tempController->value(EnumsDeclarations::TEMPS_INSIDE).toFloat();
                        bool useToggle = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE).toBool();
                        int toggleOnDuration = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_TOGGLE_ON_DURATION).toInt();

                        if (useToggle) {
                            if (insideTemp<heatingTemp) {
                                if (QDateTime::currentMSecsSinceEpoch()> m_lastHeatOff + toggleOnDuration) {
                                    // switch off
                                    m_lastHeatOff = QDateTime::currentMSecsSinceEpoch();
                                    qCDebug(LG_HEATING_LOGIC) << "Toggle off for" << toggleOnDuration;
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
                        qCWarning(LG_HEATING_LOGIC) << "Tank temp too low";
                    }
                } else {
                    qCWarning(LG_HEATING_LOGIC) << "Out of valid time";
                }
            } else {
                qCWarning(LG_HEATING_LOGIC) << "Out of valid month";
            }
        } else {
            qCWarning(LG_HEATING_LOGIC) << "Inside temp is not valid";
        }
        break;
    case EnumsDeclarations::SETTING_MODE_MANUAL:
        heatingPumpOn = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE).toBool();
        break;
    }

    m_relayController->setValue(EnumsDeclarations::RELAYS_HEATING_PUMP, heatingPumpOn, true, true);
}

bool HeatingLogic::isValidMonth() {
    int from = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MONTH_FROM).toInt();
    int orgTo = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MONTH_TO).toInt();
    int to = orgTo;

    int currentMonth = QDate::currentDate().month();
    if (to<from) {
        to+=12;
        if (currentMonth <= orgTo) currentMonth+=12;
    }

    qDebug() << Q_FUNC_INFO << currentMonth << from << to << orgTo;

    return currentMonth>=from && currentMonth<=to;
}

bool HeatingLogic::isValidTime() {
    int from = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_HOUR_FROM).toInt();
    int to = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_HOUR_TO).toInt();

    int currentHour = QTime::currentTime().hour();
    if (to<from) {
        to+=24;
    }

    return currentHour>=from && currentHour<=to;
}

bool HeatingLogic::isValidTankTemp() {
    if (m_tempController->value(EnumsDeclarations::TEMPS_TANK).isValid()) {
        double tankTemp = m_tempController->value(EnumsDeclarations::TEMPS_TANK).toDouble();
        float heatingTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_TEMP).toDouble();
        float minTankTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_HEATING_MIN_TEMP_TANK).toDouble();

        return tankTemp>heatingTemp && tankTemp>minTankTemp;
    } else {
        return true;        // hmm, well - lets not create too many deps
    }
}

void HeatingLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_HEATING_LOGIC) << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
