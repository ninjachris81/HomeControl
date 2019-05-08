#include "preheatlogic.h"
#include <QDebug>
#include <QDateTime>

PreheatLogic::PreheatLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));

    Q_ASSERT(m_tempController);
    Q_ASSERT(m_relayController);
    Q_ASSERT(m_settingsController);

    startMaintenance(1000);
}

void PreheatLogic::onMaintenance() {
    //qDebug() << Q_FUNC_INFO;

    int from = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_FROM).toInt();
    int to = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_TO).toInt();
    int currentHour = QDateTime::currentDateTime().time().hour();
    int mode = static_cast<EnumsDeclarations::SETTINGS_MODES>(m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_MODE).toInt());

    switch(mode) {
    case EnumsDeclarations::SETTING_MODE_AUTOMATIC:

        if (from <= currentHour && to >= currentHour) {
            float preheatHc = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_HC_TEMP).toFloat();
            float preheatWater = m_settingsController->value(EnumsDeclarations::SETTINGS_PREHEAT_WATER_TEMP).toFloat();

            if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_HC)) {
                float hcTemp = m_tempController->value(EnumsDeclarations::TEMPS_HC).toDouble();

                if (hcTemp<preheatHc) {
                    // turn on
                    m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, true, true);
                } else {
                    m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, false, true);
                }
            } else {
                qWarning() << "HC Value is invalid";
                m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, false, true);
            }

            if (m_tempController->valueIsValid(EnumsDeclarations::TEMPS_WATER)) {
                float waterTemp = m_tempController->value(EnumsDeclarations::TEMPS_WATER).toDouble();

                if (waterTemp<preheatWater) {
                    // turn on
                    m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, true, true);
                } else {
                    m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, false, true);
                }
            } else {
                qWarning() << "Water Value is invalid";
                m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, false, true);
            }

        } else {
            qWarning() << "Not in time frame" << from << to << currentHour;
            m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, false, true);
            m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, false, true);
        }
        break;


    case EnumsDeclarations::SETTING_MODE_MANUAL:
        qDebug() << QDateTime::currentSecsSinceEpoch() - (m_lastStartDuration + m_lastStartRequest);

        if (m_lastStartRequest>0 && m_lastStartDuration>0 && QDateTime::currentSecsSinceEpoch() < m_lastStartRequest + m_lastStartDuration) {
            m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, true, true);
            m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, true, true);
        } else {
            if (m_lastStartRequest>0) {
                m_lastStartRequest = 0;
                m_lastStartDuration = 0;
                m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC, true);
            }
            m_relayController->setValue(EnumsDeclarations::RELAYS_HC_PUMP, false, true);
            m_relayController->setValue(EnumsDeclarations::RELAYS_WATER_PUMP, false, true);
        }
        break;
    }

}

void PreheatLogic::startPreheat(qint64 duration) {
    qDebug() << Q_FUNC_INFO << duration;

    m_lastStartRequest = QDateTime::currentSecsSinceEpoch();
    m_lastStartDuration = duration;

    m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_MANUAL, true);
}

void PreheatLogic::stopPreheat() {
    qDebug() << Q_FUNC_INFO;
    m_lastStartRequest = 0;
    m_lastStartDuration = 0;
    m_settingsController->setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC, true);
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
    }
}
