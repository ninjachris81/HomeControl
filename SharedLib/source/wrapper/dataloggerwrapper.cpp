#include "include/wrapper/dataloggerwrapper.h"

DataLoggerWrapper::DataLoggerWrapper(DataLoggerController *controller) : ControllerWrapper(controller)
{
    m_tempTempModel = new DataLoggerSqlListModel(controller, QSqlDatabase::database(DataLoggerController::DB_CONN_DATA_LOG), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_TANK));
    m_hcTempModel = new DataLoggerSqlListModel(controller, QSqlDatabase::database(DataLoggerController::DB_CONN_DATA_LOG), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_HC));
    m_insideTempModel = new DataLoggerSqlListModel(controller, QSqlDatabase::database(DataLoggerController::DB_CONN_DATA_LOG), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_INSIDE));
    m_solarTempModel = new DataLoggerSqlListModel(controller, QSqlDatabase::database(DataLoggerController::DB_CONN_DATA_LOG), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_SOLAR_HC));
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
