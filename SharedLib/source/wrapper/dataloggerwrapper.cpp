#include "include/wrapper/dataloggerwrapper.h"
#include "utils/databasemanager.h"

DataLoggerWrapper::DataLoggerWrapper(DataLoggerController *controller) : ControllerWrapper(controller)
{
    m_tempTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_TANK) + " AND is_valid=true");
    m_hcTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_HC) + " AND is_valid=true");
    m_insideTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_INSIDE) + " AND is_valid=true");
    m_solarTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_SOLAR_HC) + " AND is_valid=true");

    m_pvModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::EXTERNAL_PV_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_PVS_MAMPS) + " AND is_valid=true", 0, Qt::DescendingOrder, 3000);
    m_currentBasementModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::CURRENT_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_CURRENTS_MAIN_BASEMENT) + " AND is_valid=true", 0, Qt::DescendingOrder, 600);

    m_hcPumpModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::RELAY_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_RELAYS_HC_PUMP) + " AND is_valid=true", 0, Qt::AscendingOrder);
    m_heatingPumpModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::RELAY_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_RELAYS_HEATING_PUMP) + " AND is_valid=true", 0, Qt::AscendingOrder);
    m_boilerModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::RELAY_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_RELAYS_BOILER) + " AND is_valid=true", 0, Qt::AscendingOrder);
}

DataLoggerWrapper::~DataLoggerWrapper() {
    delete m_tempTempModel;
    delete m_hcTempModel;
    delete m_insideTempModel;
    delete m_solarTempModel;

    delete m_pvModel;
    delete m_currentBasementModel;

    delete m_hcPumpModel;
    delete m_heatingPumpModel;
    delete m_boilerModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::tankTempModel() {
    return m_tempTempModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::hcTempModel() {
    return m_hcTempModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::insideTempModel() {
    return m_insideTempModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::solarTempModel() {
    return m_solarTempModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::pvModel() {
    return m_pvModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::currentBasementModel() {
    return m_currentBasementModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::hcPumpModel() {
    return m_hcPumpModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::heatingPumpModel() {
    return m_heatingPumpModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::boilerModel() {
    return m_boilerModel;
}
