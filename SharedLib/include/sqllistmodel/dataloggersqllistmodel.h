#ifndef DATALOGGERLISTMODEL_H
#define DATALOGGERLISTMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>

#include <QtCharts/QAbstractSeries>
#include <QtCharts/QDateTimeAxis>

#include "hcsqlquerymodel.h"
#include "controller/dataloggercontroller.h"

QT_CHARTS_USE_NAMESPACE

class DataLoggerSqlListModel : public HCSqlQueryModel {
    Q_OBJECT

public:
    explicit DataLoggerSqlListModel(DataLoggerController* controller, QSqlDatabase db, QString filter);

    Q_INVOKABLE void updateSeries(QAbstractSeries *series, QAbstractAxis *dtAxis);

    Q_INVOKABLE void refresh();

protected slots:
    void onDataLogChanged();

protected:
    DataLoggerController* m_controller;
    QString m_filter;

    QVariant resolveData(int colIndex, QVariant value) const;
    QVariant resolveDisplayData(const QModelIndex &item, int role, QVariant value) const;

    void _setQuery();
};

Q_DECLARE_METATYPE( DataLoggerSqlListModel* );


#endif // DATALOGGERLISTMODEL_H
