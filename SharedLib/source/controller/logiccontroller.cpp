#include "include/controller/logiccontroller.h"
#include <QDebug>

LogicController::LogicController(ControllerManager *controllerManager, uint16_t interval, QObject *parent) : QObject(parent), m_interval(interval), m_controllerManager(controllerManager)
{
    connect(&m_maintenanceTimer, &QTimer::timeout, this, &LogicController::onMaintenance);

    connect(m_controllerManager, &ControllerManager::mqttCmdReceived, this, &LogicController::onCommandReceived);
    connect(m_controllerManager, &ControllerManager::mqttConnected, this, &LogicController::onConnected);
}

void LogicController::onConnected() {
    startMaintenance();
}

void LogicController::startMaintenance() {
    if (m_interval>0) m_maintenanceTimer.start(m_interval);
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
