#include "databridge.h"
#include <QDebug>

DataBridge::DataBridge(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{
    connect(&m_controllerManager, &ControllerManager::mqttConnected, this, &DataBridge::onMqttConnected);

    m_controllerManager.registerController(&m_tempController);
    m_controllerManager.registerController(&m_logController);
    m_controllerManager.registerController(&m_relayController);
    m_controllerManager.registerController(&m_settingsController);
    m_controllerManager.init(appConfig);

    m_tempListModelController = new ControllerListModel(&m_tempController);
    m_logListModelController = new LogControllerListModel(&m_logController);
    m_relayListModelController = new RelayControllerListModel(&m_relayController);
    m_settingsListModelController = new SettingsControllerListModel(&m_settingsController);

    m_settingsControllerWrapper = new SettingsWrapper(&m_settingsController);

    ControllerWrapper::registerTypes();
}

void DataBridge::onMqttConnected() {
}

ControllerListModel* DataBridge::tempListModelController() {
    return m_tempListModelController;
}

LogControllerListModel *DataBridge::logListModelController() {
    return m_logListModelController;
}

RelayControllerListModel *DataBridge::relayListModelController() {
    return m_relayListModelController;
}

SettingsControllerListModel *DataBridge::settingsListModelController() {
    return m_settingsListModelController;
}

SettingsWrapper *DataBridge::settingsControllerWrapper() {
    return m_settingsControllerWrapper;
}
