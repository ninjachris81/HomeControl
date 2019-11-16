#include "datalogger.h"
#include <QSqlQuery>
#include <QSqlError>

QString DataLogger::DB_TABLE_LOGS = QStringLiteral("value_log");

QString DataLogger::DB_CONN_LOGS = QStringLiteral("DATA_LOGS");

Q_LOGGING_CATEGORY(LG_DATALOGGER, "DataLogger");

DataLogger::DataLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_controllerManager(controllerManager)
{
    qCDebug(LG_DATALOGGER) << Q_FUNC_INFO;

    m_entryLimit = appConfig->getInt("DATALOGGER_MAX_ENTRIES", 100000);

    m_LogTimer.setInterval(appConfig->getInt("DATALOGGER_INTERVAL_MS", 5000));
    connect(&m_LogTimer, &QTimer::timeout, this, &DataLogger::onLogData);
    m_LogTimer.start();

    m_LimitTimer.setInterval(120000);
    connect(&m_LimitTimer, &QTimer::timeout, this, &DataLogger::onCheckLimit);
    m_LimitTimer.start();


    m_db = QSqlDatabase::addDatabase( "QSQLITE", DB_CONN_LOGS );
    m_db.setDatabaseName( "data_log.db" );
    if (m_db.open()) {
        if (!m_db.tables().contains("value_log")) {
            QSqlQuery query(m_db);

            if (query.exec("CREATE TABLE " + DB_TABLE_LOGS + " (\"ts\"	UNSIGNED BIG INT NOT NULL, \"controller\"	INTEGER NOT NULL, \"valueIndex\"	INTEGER, \"val\"	REAL)")) {
                qCDebug(LG_DATALOGGER) << "Created new table" << DB_TABLE_LOGS;
            } else {
                qCWarning(LG_DATALOGGER) << "Failed to create new table" << query.lastError().text();
            }
        } else {
            qCDebug(LG_DATALOGGER) << "Table already exists";
        }
    } else {
        qCWarning(LG_DATALOGGER) << "Unable to open value log db";
    }
}

void DataLogger::registerValue(ControllerBase *controller, int valueIndex) {
    qCDebug(LG_DATALOGGER) << Q_FUNC_INFO << controller->getName() << valueIndex;
    m_Values.insert(controller, valueIndex);
}

void DataLogger::onCheckLimit() {
    qCDebug(LG_DATALOGGER) << Q_FUNC_INFO;

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM value_log WHERE rowid NOT IN ( SELECT rowid FROM value_log order by ts DESC LIMIT :limit)");
    query.bindValue(":limit", m_entryLimit);

    if (!query.exec()) {
        qCWarning(LG_DATALOGGER) << "Failed to execute delete statement" << query.lastError();
    }
}

void DataLogger::onLogData() {
    qCDebug(LG_DATALOGGER) << Q_FUNC_INFO;

    QMapIterator<ControllerBase*, int> it(m_Values);

    while(it.hasNext()) {
        it.next();

        if (it.key()->valueIsValid(it.value())) {
            QVariant v = it.key()->value(it.value());
            addEntry(it.key()->getType(), it.value(), v);
        }
    }
}

void DataLogger::addEntry(ControllerBase::CONTROLLER_TYPE controllerType, int index, QVariant v) {
    qCDebug(LG_DATALOGGER) << Q_FUNC_INFO << controllerType << index << v;

    QSqlQuery query(m_db);

    query.prepare("INSERT INTO " + DB_TABLE_LOGS + " (ts, controller, valueIndex, val) VALUES (:ts, :controller, :valueIndex, :val)");
    query.bindValue(":ts", QDateTime::currentMSecsSinceEpoch());
    query.bindValue(":controller", controllerType);
    query.bindValue(":valueIndex", index);
    query.bindValue(":val", v);

    if (!query.exec()) {
        qCWarning(LG_DATALOGGER) << query.lastError();
    }
}
