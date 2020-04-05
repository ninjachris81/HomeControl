#include "include/wrapper/dataloggerwrapper.h"

DataLoggerWrapper::DataLoggerWrapper(DataLoggerController *controller) : ControllerWrapper(controller)
{
    m_tempTempModel = new DataLoggerSqlListModel(controller, QSqlDatabase::database(DataLoggerController::DB_CONN_DATA_LOG), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_TANK));
    m_hcTempModel = new DataLoggerSqlListModel(controller, QSqlDatabase::database(DataLoggerController::DB_CONN_DATA_LOG), "controller=" + QString::number(ControllerBase::TEMP_CONTROLLER) + " AND value_index=" + QString::number(MQTT_PATH_TEMPS_HC));
}

DataLoggerWrapper::~DataLoggerWrapper() {
    delete m_tempTempModel;
    delete m_hcTempModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::tankTempModel() {
    return m_tempTempModel;
}

DataLoggerSqlListModel* DataLoggerWrapper::hcTempModel() {
    return m_hcTempModel;
}
