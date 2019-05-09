#include "include/sqllistmodel/logsqllistmodel.h"
#include <QDebug>
#include <QTimer>

LogSqlListModel::LogSqlListModel(QObject *parent, QSqlDatabase db) : SqlQueryModel(parent, db) {
    qDebug() << db;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
    } else {
        _setQuery();
    }
}

void LogSqlListModel::_setQuery() {
    qDebug() << database();

    QTimer::singleShot(2000, [=]() {
        updateTable(LogController::DB_TABLE_LOGS);
        qDebug() << "Row count" << rowCount();
        qDebug() << "Roles" << roleNames();
    } );

}

