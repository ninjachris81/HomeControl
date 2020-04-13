#ifndef DATALOGGERLISTMODEL_H
#define DATALOGGERLISTMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>

#include <QtCharts/QXYSeries>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QBarCategoryAxis>

#include "hcsqlquerymodel.h"
#include "controller/dataloggercontroller.h"

QT_CHARTS_USE_NAMESPACE

class DataLoggerSqlListModel : public HCSqlQueryModel {
    Q_OBJECT

public:
    explicit DataLoggerSqlListModel(DataLoggerController* controller, QSqlDatabase db, QString filter, int sortColumn = 0, Qt::SortOrder sortOrder = Qt::DescendingOrder, int limit = 1000);

    Q_PROPERTY(QDateTime maxDate READ maxDate NOTIFY maxDateChanged)
    Q_PROPERTY(QDateTime minDate READ minDate NOTIFY minDateChanged)
    Q_PROPERTY(int daysLimit READ daysLimit WRITE setDaysLimit NOTIFY daysLimitChanged)

    Q_INVOKABLE void updateXYSeries(QtCharts::QXYSeries *series, QAbstractAxis *dtAxis);
    Q_INVOKABLE void updateBarSeries(QAbstractAxis *valAxis, QBarCategoryAxis *catAxis, QBarSet *barSet);

    Q_INVOKABLE void refresh();

    QDateTime maxDate();
    QDateTime minDate();

    int daysLimit();
    void setDaysLimit(int daysLimit);

protected slots:
    void onDataLogChanged();

protected:
    DataLoggerController* m_controller;
    QString m_filter;
    QString m_additionalFilter;
    int m_sortColumn;
    Qt::SortOrder m_sortOrder;

    QDateTime m_maxDate;
    QDateTime m_minDate;
    int m_daysLimit = -1;

    QVariant resolveData(int colIndex, QVariant value) const;
    QVariant resolveDisplayData(const QModelIndex &item, int role, QVariant value) const;

    void _setQuery();

signals:
    void maxDateChanged();
    void minDateChanged();
    void daysLimitChanged();

};

Q_DECLARE_METATYPE( DataLoggerSqlListModel* );


#endif // DATALOGGERLISTMODEL_H
