#include "include/wrapper/logwrapper.h"
#include "utils/databasemanager.h"

#include <QTimer>

LogWrapper::LogWrapper(LogController* controller) : ControllerWrapper (controller) {

    m_logListModel = new LogSqlListModel(controller, DatabaseManager::instance()->db());
}

void LogWrapper::clearLog(int logFilter) {
    EnumsDeclarations::MQTT_CMDS cmd = EnumsDeclarations::CMD_NONE;

    switch(logFilter) {
    case 0:
        cmd = EnumsDeclarations::CMD_LOG_CLEAR_ALL;
        break;
    case 1:
        cmd = EnumsDeclarations::CMD_LOG_CLEAR_INFO;
        break;
    case 2:
        cmd = EnumsDeclarations::CMD_LOG_CLEAR_ERROR;
        break;
    case 3:
        cmd = EnumsDeclarations::CMD_LOG_CLEAR_STARTUP;
        break;
    default:
        qWarning() << "Invalid logFilter" << logFilter;
        return;
    }

    m_controller->publishCmd(cmd);
}

LogSqlListModel* LogWrapper::logListModel() {
    return m_logListModel;
}
