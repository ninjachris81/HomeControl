#include "controller/logcontroller.h"
#include "constants_qt.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QThread>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "controller/controllermanager.h"
#include "controller/settingscontroller.h"

#include "utils/databasemanager.h"

QString LogController::CONTROLLER_NAME = QStringLiteral("LogController");
QString LogController::DB_TABLE_LOGS = QStringLiteral("logs");

Q_LOGGING_CATEGORY(LG_LOG_CONTROLLER, "LogController");

LogController::LogController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
{
}

QString LogController::getName() {
    return CONTROLLER_NAME;
}

QStringList LogController::getTopicPath() {
    return QStringList() << MQTT_PATH_LOGS;
}

QStringList LogController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::LOGS_TYPE_LABELS);
}

void LogController::broadcastValues() {
    // do nothing
}

QVariant::Type LogController::getDefaultValueType() {
    return QVariant::String;
}

QString LogController::getEnumName() {
    return "MQTT_LOGS";
}

qint64 LogController::getValueLifetime(int index) {
    Q_UNUSED(index);

    return LIFETIME_UNLIMITED;
}

void LogController::onInit() {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO;

    m_db = DatabaseManager::instance()->db();
}

void LogController::onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    switch(cmd) {
    case EnumsDeclarations::CMD_LOG_CLEAR_ALL:
        clearLog();
        break;
    case EnumsDeclarations::CMD_LOG_CLEAR_INFO:
        clearLog(MQTT_PATH_LOGS_TYPE_INFO);
        break;
    case EnumsDeclarations::CMD_LOG_CLEAR_ERROR:
        clearLog(MQTT_PATH_LOGS_TYPE_ERROR);
        break;
    case EnumsDeclarations::CMD_LOG_CLEAR_STARTUP:
        clearLog(MQTT_PATH_LOGS_TYPE_STARTUP);
        break;
    default:
        break;
    }
}

void LogController::clearLog(int typeFilter) {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << typeFilter;

    if (m_parent->isServer()) {
        QSqlQuery query(m_db);

        if (typeFilter==-1) {
            if (!query.exec("DELETE FROM " + DB_TABLE_LOGS)) {
                qWarning() << "Failed to clear table" << query.lastError();
            }
        } else {
            query.prepare("DELETE FROM " + DB_TABLE_LOGS + " WHERE type=:type");
            query.bindValue(":type", typeFilter);

            if (!query.exec()) {
                qWarning() << "Failed to clear table" << query.lastError();
            }
        }
    }
}

void LogController::addLog(EnumsDeclarations::MQTT_LOGS type, QString source, QString msg) {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << type << msg;

    if (m_parent->isServer()) {
        insertRecord(QDateTime::currentDateTime(), type, source, msg);
    } else {
        setValue(type, source + MQTT_LOG_SOURCE_DIV + msg, true, true);
    }
}

void LogController::onValueChanged(int index, QVariant value) {
    qCWarning(LG_LOG_CONTROLLER) << Q_FUNC_INFO << index << value;
}

void LogController::onSetReceived(int index, QVariant value) {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << index << value;

    if (m_parent->isServer()) {
        // index = type

        int i = value.toString().indexOf(MQTT_LOG_SOURCE_DIV);
        if (i>=0) {
            QString source = value.toString().left(i);
            QString msg = value.toString().mid(i+QString(MQTT_LOG_SOURCE_DIV).length());

            qCDebug(LG_LOG_CONTROLLER) << source << msg;

            insertRecord(QDateTime::currentDateTime(), index, source, msg);
        } else {

        }
    } else {
        // do nothing
    }
}

bool LogController::insertRecord(QDateTime date, int type, QString source, QString msg) {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << m_db <<date << type << source << msg;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO " + DB_TABLE_LOGS + " (ts, type, source, msg) VALUES (:date, :type, :source, :msg)");
    query.bindValue(":date", date.toMSecsSinceEpoch());
    query.bindValue(":type", type);
    query.bindValue(":source", source);
    query.bindValue(":msg", msg);

    if (!query.exec()) {
        qCWarning(LG_LOG_CONTROLLER) << "Error while inserting log" << query.lastError().text();
        return false;
    } else {
        return true;
    }
}
