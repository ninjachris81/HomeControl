#include "include/utils/qsqldatabasewrapper.h"

QSqlDatabaseWrapper::QSqlDatabaseWrapper() : QObject() {

}

void QSqlDatabaseWrapper::setDatabaseName(const QString& name) {
    m_db.setDatabaseName(name);
}

bool QSqlDatabaseWrapper::open() {
    if (m_db.open()) {
        Q_EMIT(opened());
        return true;
    } else {
        return false;
    }
}

QSqlDatabase* QSqlDatabaseWrapper::db() {
    return &m_db;
}

void QSqlDatabaseWrapper::addDatabase(const QString &type, const QString &connectionName) {
    m_db = QSqlDatabase::addDatabase(type, connectionName);
}
