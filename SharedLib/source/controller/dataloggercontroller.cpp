#include "controller/dataloggercontroller.h"

#include <QSqlError>
#include <QSqlQuery>

#include "controller/controllermanager.h"

#include "utils/databasemanager.h"

QString DataLoggerController::CONTROLLER_NAME = QStringLiteral("DataLogController");
QString DataLoggerController::DB_TABLE_DATA_LOG = QStringLiteral("data_log");

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

        m_daysLimit = m_parent->appConfig()->getInt("DATALOGGER_MAX_DAYS", 356);

        m_LimitTimer.setInterval(120000);
        connect(&m_LimitTimer, &QTimer::timeout, this, &DataLoggerController::onCheckLimit);
        m_LimitTimer.start();
    }
}

void DataLoggerController::registerValue(ControllerBase *controller, int valueIndex) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << controller->getName() << valueIndex;
    m_Values.insert(controller, valueIndex);

    connect(controller, &ControllerBase::valueChanged, [=] (int index, QVariant value) {
        Q_UNUSED(value)
        if (index==valueIndex) this->onExternalControllerValueChanged(controller, index);
    });

    connect(controller, &ControllerBase::valueIsValid, [=] (int index) {
        if (index==valueIndex) this->onExternalControllerValueChanged(controller, index);
    });

    onExternalControllerValueChanged(controller, valueIndex);       // log once
}

void DataLoggerController::onExternalControllerValueChanged(ControllerBase* controller, int index) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << controller->getName() << index;

    addEntry(controller->getType(), index, controller->value(index), controller->valueIsValid(index));
}


void DataLoggerController::onCheckLimit() {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO;

    qint64 tsLimit = QDateTime::currentDateTime().addDays(-m_daysLimit).toMSecsSinceEpoch();

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM " + DB_TABLE_DATA_LOG + " WHERE ts<:tsLimit");
    query.bindValue(":tsLimit", tsLimit);

    if (!query.exec()) {
        qCWarning(LG_DATA_LOG_CONTROLLER) << "Failed to execute delete statement" << query.lastError();
    }
}

double DataLoggerController::toDouble(QVariant value) {
    switch (value.type()) {
    case QVariant::DateTime:
        return value.toDateTime().toMSecsSinceEpoch();
    case QVariant::Bool:
        return value.toBool() ? 1 : 0;
    case QVariant::Int:
    case QVariant::Double:
        return value.toDouble();
    default:
        qCWarning(LG_DATA_LOG_CONTROLLER) << "Cannot convert value type" << value.typeName();
        return 0;
    }
}

void DataLoggerController::addEntry(ControllerBase::CONTROLLER_TYPE controllerType, int index, QVariant v, bool isValid) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << controllerType << index << v;

    QSqlQuery query(m_db);

    query.prepare("INSERT INTO " + DB_TABLE_DATA_LOG + " (ts, controller, value_index, val, is_valid) VALUES (:ts, :controller, :value_index, :val, :is_valid)");
    query.bindValue(":ts", QDateTime::currentMSecsSinceEpoch());
    query.bindValue(":controller", controllerType);
    query.bindValue(":value_index", index);
    query.bindValue(":val", toDouble(v));
    query.bindValue(":is_valid", isValid);

    if (!query.exec()) {
        qCWarning(LG_DATA_LOG_CONTROLLER) << query.lastError();
    }
}
