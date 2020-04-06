#include "include/sqllistmodel/dataloggersqllistmodel.h"
#include <QDebug>

#include <QModelIndex>
#include <QRandomGenerator>

#include <QDateTime>

#include <QtCharts/QXYSeries>
#include <QVXYModelMapper>

DataLoggerSqlListModel::DataLoggerSqlListModel(DataLoggerController* controller, QSqlDatabase db, QString filter) : HCSqlQueryModel(db), m_controller(controller), m_filter(filter) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << db;

    connect(m_controller, &DataLoggerController::dataLogDataChanged, this, &DataLoggerSqlListModel::onDataLogChanged);

    setLimit(4320);     // 1 day
}

void DataLoggerSqlListModel::_setQuery() {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << database();
    updateTable(DataLoggerController::DB_TABLE_DATA_LOG, m_filter, 0, Qt::DescendingOrder);
}

void DataLoggerSqlListModel::onDataLogChanged() {
    select();
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << rowCount();
}

QVariant DataLoggerSqlListModel::resolveData(int colIndex, QVariant value) const {
    //qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << colIndex << value;

    switch(colIndex) {
    case 0:
        return QDateTime::fromMSecsSinceEpoch(value.toLongLong());
    default:
        return value;
    }
}

QVariant DataLoggerSqlListModel::resolveDisplayData(const QModelIndex &item, int role, QVariant value) const {
    Q_UNUSED(role)
    //qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << item << role << value;

    switch(item.column()) {
    case 0:
        //qDebug() << QDateTime::fromMSecsSinceEpoch(value.toLongLong());
        return QDateTime::fromMSecsSinceEpoch(value.toLongLong());
    default:
        return value;
    }
}

void DataLoggerSqlListModel::refresh() {
    //qDebug() << Q_FUNC_INFO;

    _setQuery();

    while(canFetchMore()) {
        qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << "Fetching more";
        fetchMore();
    }
    qCDebug(LG_DATA_LOG_CONTROLLER) << rowCount() << selectStatement() << orderByClause();
}

void DataLoggerSqlListModel::updateSeries(QAbstractSeries *series, QAbstractAxis *dtAxis) {
    //qDebug() << Q_FUNC_INFO << series;

    if (series && rowCount()>1) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);

        //xySeries->clear();

        /*
        QVXYModelMapper mapper;

        mapper.setSeries(xySeries);
        mapper.setXColumn(0);
        mapper.setYColumn(3);
        mapper.setModel(this);
        */

        //select();

        //qDebug() << "COUNT" << rowCount();

        QVector<QPointF> m_data(rowCount());
        for (int i=0;i<rowCount();i++) {
            const QModelIndex dtIndex = createIndex(i, 0);
            const QModelIndex valIndex = createIndex(i, 3);
            m_data[i] = QPointF(data(dtIndex, Qt::DisplayRole).toDateTime().toMSecsSinceEpoch(), data(valIndex, Qt::DisplayRole).toDouble());
        }

        qDebug() << m_data;

        if (dtAxis) {
            const QModelIndex lastIndex = createIndex(rowCount()-1, 0);
            QVariant fromDate = data(lastIndex, Qt::DisplayRole);
            qDebug() << "FROM" << fromDate;

            const QModelIndex firstIndex = createIndex(0, 0);
            QVariant toDate = data(firstIndex, Qt::DisplayRole);
            qDebug() << "TO" << toDate;

            dtAxis->setMin(fromDate);
            dtAxis->setMax(toDate);
        }

        xySeries->replace(m_data);

        //qDebug() << "DATA" << m_data;
    } else {
        qCWarning(LG_DATA_LOG_CONTROLLER) << "Not enough data";
    }
}
