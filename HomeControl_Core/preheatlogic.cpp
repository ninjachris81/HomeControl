#include "preheatlogic.h"
#include <QDebug>
#include <QDateTime>

PreheatLogic::PreheatLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relaisController = static_cast<RelaisController*>(controllerManager->getController(RelaisController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));

    Q_ASSERT(m_tempController);
    Q_ASSERT(m_relaisController);
    Q_ASSERT(m_settingsController);

    startMaintenance(1000);
}

void PreheatLogic::onMaintenance() {
    //qDebug() << Q_FUNC_INFO;

    int from = m_settingsController->value(SETTINGS_PREHEAT_FROM).toInt();
    int to = m_settingsController->value(SETTINGS_PREHEAT_TO).toInt();
    int currentHour = QDateTime::currentDateTime().time().hour();

    if (from <= currentHour && to >= currentHour) {
        float preheatHc = m_settingsController->value(SETTINGS_PREHEAT_HC_TEMP).toFloat();
        float preheatWater = m_settingsController->value(SETTINGS_PREHEAT_WATER_TEMP).toFloat();
        float hcTemp = m_tempController->value(TEMPS_HC).toDouble();
        float waterTemp = m_tempController->value(TEMPS_WATER).toDouble();

        if (hcTemp<preheatHc) {
            // turn on
            m_relaisController->setValue(RELAYS_HC_PUMP, true, true);
        } else {
            m_relaisController->setValue(RELAYS_HC_PUMP, false, true);
        }

        if (waterTemp<preheatWater) {
            // turn on
            m_relaisController->setValue(RELAYS_WATER_PUMP, true, true);
        } else {
            m_relaisController->setValue(RELAYS_WATER_PUMP, false, true);
        }


    } else {
        qWarning() << "Not in time frame" << from << to << currentHour;
    }

}

void PreheatLogic::onCommandReceived(MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    switch(cmd) {
    case START_PREHEAT:
        break;
    }
}
