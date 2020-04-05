#include "include/utils/databasemanager.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlDriver>

QString DatabaseManager::DATABASE_NAME = "hc";
QString DatabaseManager::DATABASE_USERNAME = "hc";
QString DatabaseManager::DATABASE_PASSWORD = "test";

DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager::DatabaseManager()
{
}

void DatabaseManager::init(AppConfiguration *appConfig) {
    qDebug() << Q_FUNC_INFO;

#ifdef WIN32
    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName("DRIVER={PostgreSQL ODBC Driver(UNICODE)};DATABASE=" + DATABASE_NAME + ";SERVER=" + appConfig->getString(AppConfiguration::MQTT_HOST, "") + ";PORT=5432");
#else
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setDatabaseName(DATABASE_NAME);
#endif

    m_db.setHostName(appConfig->getString(AppConfiguration::MQTT_HOST, ""));
    m_db.setPort(5432);
    m_db.setUserName(DATABASE_USERNAME);
    m_db.setPassword(DATABASE_PASSWORD);
    if (m_db.open()) {
        qDebug() << "Connected to database";
        qDebug() << "Supports query size" << supportsQuerySize();
    } else {
        qWarning() << "Failed to connect to database" << m_db.lastError().text();
    }
}

QSqlDatabase DatabaseManager::db() {
    return m_db;
}

bool DatabaseManager::supportsQuerySize() {
    return m_db.driver()->hasFeature(QSqlDriver::QuerySize);
}
