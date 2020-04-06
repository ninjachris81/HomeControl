#include "include/sqllistmodel/logsqllistmodel.h"
#include <QDebug>

LogSqlListModel::LogSqlListModel(LogController *logController, QSqlDatabase db) : HCSqlQueryModel(db), m_logController(logController) {
    qCDebug(LG_LOG_CONTROLLER) << db;

    if (!db.isOpen()) {
        qCWarning(LG_LOG_CONTROLLER) << "Database is not open";
    } else {
        _setQuery();
    }
}

void LogSqlListModel::_setQuery() {
    qCDebug(LG_LOG_CONTROLLER) << database();
    updateTable(LogController::DB_TABLE_LOGS, "", 0, Qt::DescendingOrder);
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

void LogSqlListModel::setSourceFilter(QString filter) {
    if (filter.isEmpty()) {
        setFilter("");
    } else {
        setFilter("source='" +filter + "'");
    }
}

