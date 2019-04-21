#include "databridge.h"
#include <QDebug>

DataBridge::DataBridge(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{
    connect(&m_controllerManager, &ControllerManager::mqttConnected, this, &DataBridge::onMqttConnected);

    m_controllerManager.registerController(&m_tempController);
    m_controllerManager.registerController(&m_errorController);
    m_controllerManager.init(appConfig);

    m_tempListModelController = new ControllerListModel(&m_tempController);
    m_errorListModelController = new ErrorControllerListModel(&m_errorController);
}

void DataBridge::onMqttConnected() {
}

ControllerListModel* DataBridge::tempListModelController() {
    return m_tempListModelController;
}

ErrorControllerListModel *DataBridge::errorListModelController() {
    return m_errorListModelController;
}
