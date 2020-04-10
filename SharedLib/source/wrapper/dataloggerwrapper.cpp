#include "include/wrapper/dataloggerwrapper.h"
#include "utils/databasemanager.h"

DataLoggerWrapper::DataLoggerWrapper(DataLoggerController *controller) : ControllerWrapper(controller)
{
    m_tempTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_TANK) + " AND is_valid=true");
    m_hcTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_HC) + " AND is_valid=true");
    m_insideTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_INSIDE) + " AND is_valid=true");
    m_solarTempModel = new DataLoggerSqlListModel(controller, DatabaseManager::instance()->db(), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_SOLAR_HC) + " AND is_valid=true");
}

DataLoggerWrapper::~DataLoggerWrapper() {
    delete m_tempTempModel;
    delete m_hcTempModel;
    delete m_insideTempModel;
    delete m_solarTempModel;
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