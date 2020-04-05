#include "controller/dataloggercontroller.h"

#include <QSqlError>
#include <QSqlQuery>

#include "controller/controllermanager.h"

#include "utils/databasemanager.h"

QString DataLoggerController::CONTROLLER_NAME = QStringLiteral("DataLogController");
QString DataLoggerController::DB_TABLE_DATA_LOG = QStringLiteral("data_log");
QString DataLoggerController::DB_CONN_DATA_LOG = QStringLiteral("HC_VALUE_LOG");

Q_LOGGING_CATEGORY(LG_DATA_LOG_CONTROLLER, "DataLogController");

DataLoggerController::DataLoggerController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
{

}

QString DataLoggerController::getName() {
    return CONTROLLER_NAME;
}

QStringList DataLoggerController::getTopicPath() {
    return QStringList() << "";
}

QStringList DataLoggerController::getLabelList() {
    return QStringList();
}

QVariant::Type DataLoggerController::getDefaultValueType() {
    return QVariant::String;
}

QString DataLoggerController::getEnumName() {
    return "";
}

qint64 DataLoggerController::getValueLifetime(int index) {
    Q_UNUSED(index);
    return LIFETIME_UNLIMITED;
}

void DataLoggerController::onInit() {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO;

    m_db = DatabaseManager::instance()->db();

    if (m_parent->isServer()) {
        qCDebug(LG_DATA_LOG_CONTROLLER) << "Server DB";

        m_entryLimit = m_parent->appConfig()->getInt("DATALOGGER_MAX_ENTRIES", 100000);

        m_LogTimer.setInterval(m_parent->appConfig()->getInt("DATALOGGER_INTERVAL_MS", 20000));
        connect(&m_LogTimer, &QTimer::timeout, this, &DataLoggerController::onLogData);
        m_LogTimer.start();

        m_LimitTimer.setInterval(120000);
        connect(&m_LimitTimer, &QTimer::timeout, this, &DataLoggerController::onCheckLimit);
        m_LimitTimer.start();
    }
}

void DataLoggerController::registerValue(ControllerBase *controller, int valueIndex) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << controller->getName() << valueIndex;
    m_Values.insert(controller, valueIndex);
}


void DataLoggerController::onCheckLimit() {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO;

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM " + DB_TABLE_DATA_LOG + " WHERE rowid NOT IN ( SELECT rowid FROM " + DB_TABLE_DATA_LOG + " order by ts DESC LIMIT :limit)");
    query.bindValue(":limit", m_entryLimit);

    if (!query.exec()) {
        qCWarning(LG_DATA_LOG_CONTROLLER) << "Failed to execute delete statement" << query.lastError();
    }
}

void DataLoggerController::onLogData() {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO;

    QMapIterator<ControllerBase*, int> it(m_Values);

    while(it.hasNext()) {
        it.next();

        QVariant v = it.key()->value(it.value());
        addEntry(it.key()->getType(), it.value(), v, it.key()->valueIsValid(it.value()));
    }
}

void DataLoggerController::addEntry(ControllerBase::CONTROLLER_TYPE controllerType, int index, QVariant v, bool isValid) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << controllerType << index << v;

    QSqlQuery query(m_db);

    query.prepare("INSERT INTO " + DB_TABLE_DATA_LOG + " (ts, controller, value_index, val, is_valid) VALUES (:ts, :controller, :value_index, :val, :is_valid)");
    query.bindValue(":ts", QDateTime::currentMSecsSinceEpoch());
    query.bindValue(":controller", controllerType);
    query.bindValue(":value_index", index);
    query.bindValue(":val", v);
    query.bindValue(":is_valid", isValid);

    if (!query.exec()) {
        qCWarning(LG_DATA_LOG_CONTROLLER) << query.lastError();
    }
}
