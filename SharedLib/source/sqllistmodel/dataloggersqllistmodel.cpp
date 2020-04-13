#include "include/sqllistmodel/dataloggersqllistmodel.h"
#include <QDebug>

#include <QModelIndex>
#include <QRandomGenerator>

#include <QDateTime>

#include <QtCharts/QBarSet>

DataLoggerSqlListModel::DataLoggerSqlListModel(DataLoggerController* controller, QSqlDatabase db, QString filter, int sortColumn, Qt::SortOrder sortOrder, int limit) : HCSqlQueryModel(db), m_controller(controller), m_filter(filter), m_sortColumn(sortColumn), m_sortOrder(sortOrder) {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << db;

    m_additionalFilter = "";
    connect(m_controller, &DataLoggerController::dataLogDataChanged, this, &DataLoggerSqlListModel::onDataLogChanged);

    setLimit(limit);
}

void DataLoggerSqlListModel::_setQuery() {
    qCDebug(LG_DATA_LOG_CONTROLLER) << Q_FUNC_INFO << database();
    updateTable(DataLoggerController::DB_TABLE_DATA_LOG, m_filter + m_additionalFilter, m_sortColumn, m_sortOrder);
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

void DataLoggerSqlListModel::updateXYSeries(QXYSeries *series, QAbstractAxis *dtAxis) {
    //qDebug() << Q_FUNC_INFO << series;

    if (series && rowCount()>1) {
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

        //qDebug() << m_data;

        const QModelIndex lastIndex = createIndex(rowCount()-1, 0);
        QVariant fromDate = data(lastIndex, Qt::DisplayRole);
        const QModelIndex firstIndex = createIndex(0, 0);
        QVariant toDate = data(firstIndex, Qt::DisplayRole);

        m_minDate = fromDate.toDateTime();
        m_maxDate = toDate.toDateTime();
        Q_EMIT(minDateChanged());
        Q_EMIT(maxDateChanged());


        if (dtAxis) {
            qDebug() << "FROM" << fromDate;
            qDebug() << "TO" << toDate;

            dtAxis->setMin(fromDate);
            dtAxis->setMax(toDate);
        }

        series->replace(m_data);

        //qDebug() << "DATA" << m_data;
    } else {
        qCWarning(LG_DATA_LOG_CONTROLLER) << "Not enough data";
    }
}

void DataLoggerSqlListModel::updateBarSeries(QAbstractAxis *valAxis, QBarCategoryAxis* catAxis, QBarSet *barSet) {
    qDebug() << Q_FUNC_INFO << valAxis << catAxis << barSet;

    if (valAxis && barSet && catAxis) {
        catAxis->clear();
        barSet->remove(0, barSet->count());

        QDateTime from = QDateTime(QDateTime::currentDateTime().addDays(-m_daysLimit).date());

        QDateTime tmpDate(QDateTime::currentDateTime());

        QStringList categories;
        QList<qreal> values;
        qreal maxValue = 0;

        for (int i=0;i<m_daysLimit;i++) {
            qreal currentNetTime = 0;
            qint64 currentStart = 0;
            int currentVal = -1;

            for (int o=0;o<rowCount();o++) {
                const QModelIndex dtIndex = createIndex(o, 0);
                const QModelIndex valIndex = createIndex(o, 3);
                QDateTime ct = data(dtIndex, Qt::DisplayRole).toDateTime();
                bool val = data(valIndex, Qt::DisplayRole).toDouble()==1.0;

                if (ct.date().daysTo(tmpDate.date())==0) {
                    if (val && (currentVal==-1 || currentVal==0)) {
                        // start
                        currentStart = ct.toMSecsSinceEpoch();
                    } else if (!val && currentVal==1) {
                        // end
                        currentNetTime+=(ct.toMSecsSinceEpoch() - currentStart);
                    }
                    currentVal = val?1:0;
                }
            }

            currentNetTime = currentNetTime / 1000 / 60;
            if (currentNetTime>maxValue) maxValue = currentNetTime;
            categories.insert(0, tmpDate.date().toString("ddd"));
            values.insert(0, currentNetTime);

            /*
            qDebug() << tmpDate;
            qDebug() << categories;
            qDebug() << values;
            */

            tmpDate = tmpDate.addDays(-1);
        }

        qDebug() << categories;
        qDebug() << values;
        qDebug() << maxValue;

        valAxis->setMax(maxValue);
        catAxis->append(categories);
        barSet->append(values);
    }
}

QDateTime DataLoggerSqlListModel::maxDate() {
    return m_maxDate;
}

QDateTime DataLoggerSqlListModel::minDate() {
    return m_minDate;
}

int DataLoggerSqlListModel::daysLimit() {
    return m_daysLimit;
}

void DataLoggerSqlListModel::setDaysLimit(int daysLimit) {
    m_daysLimit = daysLimit;

    QDateTime from = QDateTime(QDateTime::currentDateTime().addDays(-daysLimit).date());
    m_additionalFilter = " AND ts>" + QString::number(from.toMSecsSinceEpoch());
    Q_EMIT(daysLimitChanged());
}
