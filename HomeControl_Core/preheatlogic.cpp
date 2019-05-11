#include "preheatlogic.h"
#include <QDebug>
#include <QDateTime>

PreheatLogic::PreheatLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, PREHEAT_LOGIC_INTERVAL, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));
    m_logController = static_cast<LogController*>(controllerManager->getController(LogController::CONTROLLER_NAME));

    Q_ASSERT(m_tempController);
    Q_ASSERT(m_relayController);
    Q_ASSERT(m_settingsController);
    Q_ASSERT(m_logController);
}

void PreheatLogic::startMaintenance() {
    // reset to automatic by default
    m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC, true);
    LogicController::startMaintenance();
}


void PreheatLogic::onMaintenance() {
    qDebug() << Q_FUNC_INFO;

    int preheatFrom = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_FROM).toInt();
    int preheatTo = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_TO).toInt();
    int currentHour = QDateTime::currentDateTime().time().hour();
    int mode = static_cast<EnumsDeclarations::SETTINGS_MODES>(m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_MODE).toInt());

    bool hcOn = false;
    bool waterOn = false;

    switch(mode) {
    case EnumsDeclarations::SETTING_MODE_AUTOMATIC:

        if (preheatFrom <= currentHour && preheatTo >= currentHour) {
            float preheatHc = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_HC_TEMP).toFloat();
            float preheatWater = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_WATER_TEMP).toFloat();

            if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_HC)) {
                hcOn = m_tempController->value(EnumsDeclarations::TEMPS_HC).toDouble()<preheatHc;
            } else {
                qWarning() << "HC Value is invalid";
            }

            if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_WATER)) {
                waterOn = m_tempController->value(EnumsDeclarations::TEMPS_WATER).toDouble()<preheatWater;
            } else {
                qWarning() << "Water Value is invalid";
            }

        } else {
            int preheatStbFrom = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_STANDBY_FROM).toInt();
            int preheatStbTo = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_STANDBY_TO).toInt();

            if (preheatStbFrom <= currentHour && preheatStbTo >= currentHour) {
                if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_HC)) {
                    hcOn = m_tempController->value(EnumsDeclarations::TEMPS_HC).toDouble()<m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_HC_STANDBY_TEMP).toFloat();
                }
            } else {
                qDebug() << "Not in time frame" << preheatFrom << preheatTo << currentHour;
            }
        }
        break;


    case EnumsDeclarations::SETTING_MODE_MANUAL:
        if (m_lastStartRequest>0 && m_lastStartDuration>0 && QDateTime::currentSecsSinceEpoch() < m_lastStartRequest + m_lastStartDuration) {
            hcOn = true;
            waterOn = true;
        } else {
            if (m_lastStartRequest>0) {
                m_lastStartRequest = 0;
                m_lastStartDuration = 0;
                m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC, true);
            }
        }
        break;
    }

    m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, hcOn, true);
    m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, waterOn, true);
}

void PreheatLogic::startPreheat(qint64 duration) {
    qDebug() << Q_FUNC_INFO << duration;

    m_lastStartRequest = QDateTime::currentSecsSinceEpoch();
    m_lastStartDuration = duration;

    m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_MANUAL, true);

    m_logController->addLog(EnumsDeclarations::LOGS_TYPE_INFO, DEV_ID_SERVER, tr("Started preheat"));
}

void PreheatLogic::stopPreheat() {
    qDebug() << Q_FUNC_INFO;
    m_lastStartRequest = 0;
    m_lastStartDuration = 0;
    m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC, true);

    m_logController->addLog(EnumsDeclarations::LOGS_TYPE_INFO, DEV_ID_SERVER, tr("Stopped preheat"));
}

void PreheatLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    switch(cmd) {
    case EnumsDeclarations::CMD_START_PREHEAT:
        startPreheat(m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_DURATION).toInt());
        break;
    case EnumsDeclarations::CMD_STOP_PREHEAT:
        stopPreheat();
        break;
    default:
        break;
    }
}
