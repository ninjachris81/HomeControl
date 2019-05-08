#include "include/controller/logcontroller.h"
#include "include/constants_qt.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "include/controller/controllermanager.h"
#include "include/controller/settingscontroller.h"

QString LogController::CONTROLLER_NAME = QStringLiteral("LogController");
QString LogController::DB_TABLE_LOGS = QStringLiteral("logs");

LogController::LogController(QObject *parent) : ControllerBase(parent)
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

QVariant::Type LogController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::StringList;
}

bool LogController::isValueOwner(int index) {
    Q_UNUSED(index);

    return true;
}

void LogController::broadcastValues() {
    // do nothing
}

qint64 LogController::getValueLifetime(int index) {
    Q_UNUSED(index);

    return LIFETIME_UNLIMITED;
}

void LogController::onInit() {
    if (m_mode==ControllerBase::VALUE_OWNER_SERVER) {
        m_db = QSqlDatabase::addDatabase("QSQLITE" , "HC_LOGS");
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
    }
}

bool LogController::checkTables() {
    qDebug() << Q_FUNC_INFO;

    QStringList tables = m_db.tables();

    if (!tables.contains(DB_TABLE_LOGS)) {
        qDebug() << "Creating new table" << DB_TABLE_LOGS;

        QSqlQuery query(m_db);
        if (query.exec("CREATE TABLE " + DB_TABLE_LOGS + " (\"date\" DATETIME NOT NULL, \"type\" TINYINT NOT NULL, \"msg\" TEXT)")) {
            qDebug() << "Created new table" << DB_TABLE_LOGS;
        } else {
            qWarning() << "Failed to create new table" << query.lastError().text();
            return false;
        }
    }

    return true;
}

void LogController::onSettingsValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (index==EnumsDeclarations::SETTINGS_CORE_HOST) {
        if (m_db.isOpen()) {
            m_db.close();
        }

        retrieveLog(value.toString());
    }
}

void LogController::retrieveLog(QString host) {
    qDebug() << Q_FUNC_INFO << host;

    m_db = QSqlDatabase::addDatabase("QSQLITE" , "HC_LOGS");
    m_db.setDatabaseName(":memory:");

    if (m_db.open()) {
        checkTables();

        QTcpSocket socket;
        socket.connectToHost(host, LOG_PORT);
        if (socket.waitForConnected(2000)) {
            socket.waitForReadyRead(2000);
            QByteArray buffer = socket.readAll();
            socket.close();

            QJsonDocument doc = QJsonDocument::fromBinaryData(buffer);
            QJsonArray arr = doc.array();

            qDebug() << arr;

            for (int i=0;i<arr.count();i++) {
                QJsonArray data = arr.at(i).toArray();
                insertRecord(QDateTime::fromString(data.at(0).toString()), data.at(1).toInt(), data.at(2).toString());
            }
        } else {
            qWarning() << "Timeout connecting to" << host;
        }
    } else {
        qWarning() << "Failed to open mem database" << m_db.lastError().text();
    }
}

void LogController::onNewConnection() {
    qDebug() << Q_FUNC_INFO;

    QTcpSocket *socket = m_tcpServer->nextPendingConnection();

    QJsonArray array;

    QSqlQuery query("SELECT * FROM " + DB_TABLE_LOGS, m_db);
    while (query.next()) {
        QJsonArray data;

        data.append(QJsonValue(query.value("date").toDateTime().toString()));
        data.append(QJsonValue(query.value("type").toInt()));
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
    qWarning() << Q_FUNC_INFO << index << value;
}

void LogController::onSetReceived(int index, QVariant value) {
    if (m_mode==VALUE_OWNER_SERVER) {
        // index = type

        insertRecord(QDateTime::currentDateTime(), index, value.toString());
    } else {
        // do nothing
    }
}

bool LogController::insertRecord(QDateTime date, int type, QString msg) {
    qDebug() << date << type << msg;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO " + DB_TABLE_LOGS + " (date, type, msg) VALUES (:date, :type, :msg)");
    query.bindValue(":date", date);
    query.bindValue(":type", type);
    query.bindValue(":msg", msg);

    if (!query.exec()) {
        qWarning() << "Error while inserting log" << query.lastError().text();
        return false;
    } else {
        return true;
    }
}
