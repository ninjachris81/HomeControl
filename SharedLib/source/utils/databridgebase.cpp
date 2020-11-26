#include "utils/databridgebase.h"
#include <QDebug>

DataBridgeBase::DataBridgeBase(AppConfiguration *appConfig, QString deviceId, QObject *parent) : QObject(parent), m_appConfig(appConfig), m_controllerManager(deviceId)
{
    connect(&m_controllerManager, &ControllerManager::mqttConnected, this, &DataBridgeBase::onMqttConnected);

    m_controllerManager.registerController(&m_tempController);
    m_controllerManager.registerController(&m_logController);
    m_controllerManager.registerController(&m_relayController);
    m_controllerManager.registerController(&m_settingsController);
    m_controllerManager.registerController(&m_brightnessController);
    m_controllerManager.registerController(&m_switchController);
    m_controllerManager.registerController(&m_infoController);
    m_controllerManager.registerController(&m_dataLoggerController);
    m_controllerManager.registerController(&m_pvController);
    m_controllerManager.registerController(&m_humidityController);
    m_controllerManager.registerController(&m_currentController);
    m_controllerManager.registerController(&m_powerController);
    m_controllerManager.init(appConfig);

    m_tempListModelController = new ControllerListModel(&m_tempController);
    m_logListModelController = new ControllerListModel(&m_logController);
    m_relayListModelController = new ControllerListModel(&m_relayController);
    m_settingsListModelController = new ControllerListModel(&m_settingsController);
    m_brightnessListModelController = new ControllerListModel(&m_brightnessController);
    m_switchListModelController = new ControllerListModel(&m_switchController);
    m_pvListModelController = new ControllerListModel(&m_pvController);
    m_humidityListModelController = new ControllerListModel(&m_humidityController);
    m_currentListModelController = new ControllerListModel(&m_currentController);
    m_powerListModelController = new ControllerListModel(&m_powerController);

    m_settingsControllerWrapper = new SettingsWrapper(&m_settingsController);
    m_logControllerWrapper = new LogWrapper(&m_logController);
    m_infoControllerWrapper = new InfoWrapper(&m_infoController, &m_settingsController);
    m_dataLoggerWrapper = new DataLoggerWrapper(&m_dataLoggerController);

    m_wfcManager = new WeatherForecastManager(appConfig->getString("WFC_API_KEY", ""));

    ControllerWrapper::registerTypes();
}

bool DataBridgeBase::isConnected() {
    return m_isConnected;
}

void DataBridgeBase::onMqttConnected() {
    //m_logController.addLog(EnumsDeclarations::LOGS_TYPE_STARTUP, m_controllerManager.deviceId());
    m_isConnected = true;
    Q_EMIT(isConnectedChanged());
}

ControllerListModel* DataBridgeBase::tempListModelController() {
    return m_tempListModelController;
}

ControllerListModel *DataBridgeBase::logListModelController() {
    return m_logListModelController;
}

ControllerListModel *DataBridgeBase::relayListModelController() {
    return m_relayListModelController;
}

ControllerListModel *DataBridgeBase::settingsListModelController() {
    return m_settingsListModelController;
}

ControllerListModel* DataBridgeBase::brightnessListModelController() {
    return m_brightnessListModelController;
}

ControllerListModel* DataBridgeBase::switchListModelController() {
    return m_switchListModelController;
}

ControllerListModel* DataBridgeBase::pvListModelController() {
    return m_pvListModelController;
}

ControllerListModel* DataBridgeBase::humidityListModelController() {
    return m_humidityListModelController;
}

ControllerListModel* DataBridgeBase::currentListModelController() {
    return m_currentListModelController;
}

ControllerListModel* DataBridgeBase::powerListModelController() {
    return m_powerListModelController;
}

SettingsWrapper *DataBridgeBase::settingsControllerWrapper() {
    return m_settingsControllerWrapper;
}

LogWrapper *DataBridgeBase::logControllerWrapper() {
    return m_logControllerWrapper;
}

InfoWrapper *DataBridgeBase::infoControllerWrapper() {
    return m_infoControllerWrapper;
}

DataLoggerWrapper *DataBridgeBase::dataLoggerControllerWrapper() {
    return m_dataLoggerWrapper;
}

QVariantMap DataBridgeBase::appConfigModel() {
    QVariantMap returnList;

    for (QString key : m_appConfig->keys()) {
        returnList.insert(key, m_appConfig->getValue(key, QVariant()));
    }

    return returnList;
}

QVariant DataBridgeBase::getAppConfig(QString key, QVariant defaultValue) {
    return m_appConfig->getValue(key, defaultValue);
}

void DataBridgeBase::setAppConfig(QString key, QVariant value) {
    m_appConfig->updateValue(key, value);
}
