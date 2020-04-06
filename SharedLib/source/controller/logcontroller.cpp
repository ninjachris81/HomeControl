#include "include/controller/logcontroller.h"
#include "include/constants_qt.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QThread>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "include/controller/controllermanager.h"
#include "include/controller/settingscontroller.h"

QString LogController::CONTROLLER_NAME = QStringLiteral("LogController");
QString LogController::DB_TABLE_LOGS = QStringLiteral("logs");
QString LogController::DB_CONN_LOGS = QStringLiteral("HC_LOGS");

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

    if (m_parent->isServer()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE" , DB_CONN_LOGS);
        m_db.setDatabaseName("hc_logs.db");
        if (m_db.open()) {
            checkTables();
        } else {
            qWarning() << "Failed to open db" << m_db.lastError().text();
        }

        m_tcpServer = new QTcpServer();
        connect(m_tcpServer, &QTcpServer::newConnection, this, &LogController::onNewConnection);
        if (!m_tcpServer->listen(QHostAddress::Any, LOG_PORT)) {
            qWarning() << "Failed to start log server on port" << LOG_PORT << m_tcpServer->errorString();
        }

    } else {
        SettingsController* settingsController = static_cast<SettingsController*>(m_parent->getController(SettingsController::CONTROLLER_NAME));
        connect(settingsController, &SettingsController::valueChanged, this, &LogController::onSettingsValueChanged);

        m_db = QSqlDatabase::addDatabase("QSQLITE" , DB_CONN_LOGS);
        m_db.setDatabaseName(":memory:");
        if (m_db.open()) {
            checkTables();
        } else {
            qWarning() << "Failed to open mem database" << m_db.lastError().text();
        }
    }
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

bool LogController::checkTables() {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO;

    QStringList tables = m_db.tables();

    if (!tables.contains(DB_TABLE_LOGS)) {
        qCDebug(LG_LOG_CONTROLLER) << "Creating new table" << DB_TABLE_LOGS;

        QSqlQuery query(m_db);
        if (query.exec("CREATE TABLE " + DB_TABLE_LOGS + " (\"date\" UNSIGNED BIG INT NOT NULL, \"type\" TINYINT NOT NULL, \"source\" TEXT, \"msg\" TEXT)")) {
            qCDebug(LG_LOG_CONTROLLER) << "Created new table" << DB_TABLE_LOGS;
        } else {
            qCWarning(LG_LOG_CONTROLLER) << "Failed to create new table" << query.lastError().text();
            return false;
        }
    }

    return true;
}

void LogController::onSettingsValueChanged(int index, QVariant value) {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << index << value;

    if (index==EnumsDeclarations::SETTINGS_CORE_HOST) {
        refreshLog();
    }
}

void LogController::refreshLog() {
    QSqlQuery query("DELETE FROM " + DB_TABLE_LOGS, m_db);
    if (query.exec()) {
        retrieveLog();
    } else {
        qWarning() << "Unable to truncate table" << query.lastError();
    }
}

void LogController::addLog(EnumsDeclarations::MQTT_LOGS type, QString source, QString msg) {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << type << msg;

    if (m_parent->isServer()) {
        insertRecord(QDateTime::currentDateTime(), type, source, msg);
        Q_EMIT(logDataChanged());
    } else {
        setValue(type, source + MQTT_LOG_SOURCE_DIV + msg, true, true);
    }
}

void LogController::retrieveLog() {
    QString host = static_cast<SettingsController*>(m_parent->getController(SettingsController::CONTROLLER_NAME))->value(EnumsDeclarations::SETTINGS_CORE_HOST).toString();
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO << host;

    QTcpSocket socket;
    socket.connectToHost(host, LOG_PORT);
    if (socket.waitForConnected(5000)) {
        socket.setReadBufferSize(1000 * 1024);

        if (socket.waitForReadyRead(5000)) {

            socket.waitForDisconnected(5000);

            QByteArray buffer = socket.readAll();

            socket.close();

            if (!buffer.isEmpty()) {
                QJsonDocument doc = QJsonDocument::fromBinaryData(buffer);
                if (!doc.isNull() && !doc.isEmpty()) {
                    QJsonArray arr = doc.array();

                    qCDebug(LG_LOG_CONTROLLER) << arr;

                    for (int i=0;i<arr.count();i++) {
                        QJsonArray data = arr.at(i).toArray();
                        insertRecord(QDateTime::fromSecsSinceEpoch(data.at(0).toInt()), data.at(1).toInt(), data.at(2).toString(), data.at(3).toString());
                    }

                    Q_EMIT(logDataChanged());
                } else {
                    qCWarning(LG_LOG_CONTROLLER) << "Doc is empty" << buffer.size();
                }
            } else {
                qCWarning(LG_LOG_CONTROLLER) << "Result is empty";
            }
        } else {
            qCWarning(LG_LOG_CONTROLLER) << "Timeout reading from" << host;
        }
    } else {
        qCWarning(LG_LOG_CONTROLLER) << "Timeout connecting to" << host;
    }
}

void LogController::onNewConnection() {
    qCDebug(LG_LOG_CONTROLLER) << Q_FUNC_INFO;

    QTcpSocket *socket = m_tcpServer->nextPendingConnection();

    QJsonArray array;

    QSqlQuery query("SELECT * FROM " + DB_TABLE_LOGS + " ORDER BY date DESC LIMIT 500", m_db);
    while (query.next()) {
        QJsonArray data;

        data.append(QJsonValue(query.value("date").toLongLong()));
        data.append(QJsonValue(query.value("type").toInt()));
        data.append(QJsonValue(query.value("source").toString()));
        data.append(QJsonValue(query.value("msg").toString()));

        array.append(data);
    }

    QJsonDocument doc(array);
    socket->write(doc.toBinaryData());
    socket->flush();
    socket->waitForBytesWritten();
    socket->close();
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
    query.prepare("INSERT INTO " + DB_TABLE_LOGS + " (date, type, source, msg) VALUES (:date, :type, :source, :msg)");
    query.bindValue(":date", date.toSecsSinceEpoch());
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
