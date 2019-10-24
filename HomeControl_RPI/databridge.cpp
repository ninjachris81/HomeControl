#include "databridge.h"
#include <QDebug>

DataBridge::DataBridge(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{
    connect(&m_controllerManager, &ControllerManager::mqttConnected, this, &DataBridge::onMqttConnected);

    m_controllerManager.registerController(&m_tempController);
    m_controllerManager.registerController(&m_logController);
    m_controllerManager.registerController(&m_relayController);
    m_controllerManager.registerController(&m_settingsController);
    m_controllerManager.registerController(&m_brightnessController);
    m_controllerManager.registerController(&m_switchController);
    m_controllerManager.registerController(&m_infoController);
    m_controllerManager.init(appConfig);

    m_tempListModelController = new ControllerListModel(&m_tempController);
    m_logListModelController = new LogControllerListModel(&m_logController);
    m_relayListModelController = new RelayControllerListModel(&m_relayController);
    m_settingsListModelController = new SettingsControllerListModel(&m_settingsController);
    m_brightnessListModelController = new ControllerListModel(&m_brightnessController);
    m_switchListModelController = new ControllerListModel(&m_switchController);

    m_settingsControllerWrapper = new SettingsWrapper(&m_settingsController);
    m_logControllerWrapper = new LogWrapper(&m_logController);
    m_infoControllerWrapper = new InfoWrapper(&m_infoController);

    m_logListModel = new LogSqlListModel(&m_logController, QSqlDatabase::database(LogController::DB_CONN_LOGS));

    ControllerWrapper::registerTypes();
}

bool DataBridge::isConnected() {
    return m_isConnected;
}

void DataBridge::onMqttConnected() {
    m_logController.addLog(EnumsDeclarations::LOGS_TYPE_STARTUP, DEV_ID_TERMINAL);
    m_isConnected = true;
    Q_EMIT(isConnectedChanged());
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

ControllerListModel* DataBridge::brightnessListModelController() {
    return m_brightnessListModelController;
}

ControllerListModel* DataBridge::switchListModelController() {
    return m_switchListModelController;
}

SettingsWrapper *DataBridge::settingsControllerWrapper() {
    return m_settingsControllerWrapper;
}

LogWrapper *DataBridge::logControllerWrapper() {
    return m_logControllerWrapper;
}

InfoWrapper *DataBridge::infoControllerWrapper() {
    return m_infoControllerWrapper;
}

LogSqlListModel *DataBridge::logListModel() {
    return m_logListModel;
}

QVariantMap DataBridge::appConfigModel() {
    QVariantMap returnList;

    for (QString key : m_appConfig->keys()) {
        returnList.insert(key, m_appConfig->getValue(key, QVariant()));
    }

    return returnList;
}

QVariant DataBridge::getAppConfig(QString key, QVariant defaultValue) {
    return m_appConfig->getValue(key, defaultValue);
}

void DataBridge::setAppConfig(QString key, QVariant value) {
    m_appConfig->updateValue(key, value);
}
