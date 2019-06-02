#include "include/sqllistmodel/logsqllistmodel.h"
#include <QDebug>
#include <QTimer>

LogSqlListModel::LogSqlListModel(LogController *logController, QSqlDatabase db) : SqlQueryModel(db), m_logController(logController) {
    qDebug() << db;

    connect(m_logController, &LogController::logDataChanged, this, &LogSqlListModel::onLogChanged);

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
    } else {
        _setQuery();
    }
}

void LogSqlListModel::_setQuery() {
    qDebug() << database();
    updateTable(LogController::DB_TABLE_LOGS);
    setSort(0, Qt::DescendingOrder);
}

void LogSqlListModel::onLogChanged() {
    select();
}

QVariant LogSqlListModel::resolveData(int colIndex, QVariant value) const {
    switch(colIndex) {
    case 0: {
        QDateTime d = QDateTime::fromSecsSinceEpoch(value.toLongLong());
        if (QDateTime::currentDateTime().date().daysTo(d.date())==0) {
            return tr("Today") + ", " + d.time().toString();
        } else if (qAbs(QDateTime::currentDateTime().date().daysTo(d.date()))==1) {
            return tr("Yesterday") + ", " + d.time().toString();
        } else {
            return d.toString();
        }
    }
    case 1:
        switch(value.toInt()) {
        case MQTT_PATH_LOGS_TYPE_INFO: return tr(LOGS_LABEL_INFO);
        case MQTT_PATH_LOGS_TYPE_ERROR: return tr(LOGS_LABEL_ERROR);
        case MQTT_PATH_LOGS_TYPE_STARTUP: return tr(LOGS_LABEL_STARTUP);
        default: return tr("Unknown") + " " + QString::number(value.toInt());
        }
    default:
        return value;
    }
}

void LogSqlListModel::setTypeFilter(int filter) {
    if (filter==0) {
        setFilter("");
    } else {
        setFilter("type=" + QString::number(filter-1));
    }
}
