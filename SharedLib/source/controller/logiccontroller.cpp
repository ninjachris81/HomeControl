#include "include/controller/logiccontroller.h"
#include <QDebug>

LogicController::LogicController(ControllerManager *controllerManager, QObject *parent) : QObject(parent), m_controllerManager(controllerManager)
{
    connect(&m_maintenanceTimer, &QTimer::timeout, this, &LogicController::onMaintenance);

    connect(m_controllerManager, &ControllerManager::mqttCmdReceived, this, &LogicController::onCommandReceived);
}

void LogicController::startMaintenance(uint16_t interval) {
    m_maintenanceTimer.start(interval);
}

void LogicController::stopMaintenance() {
    m_maintenanceTimer.stop();
}

void LogicController::onMaintenance() {
    qDebug() << Q_FUNC_INFO;
}

void LogicController::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    Q_UNUSED(cmd);

    qDebug() << Q_FUNC_INFO;
}
