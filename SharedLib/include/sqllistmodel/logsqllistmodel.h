#ifndef LOGSQLLISTMODEL_H
#define LOGSQLLISTMODEL_H

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>

#include "hcsqlquerymodel.h"
#include "controller/logcontroller.h"

class LogSqlListModel : public HCSqlQueryModel {
Q_OBJECT

public:
    explicit LogSqlListModel(LogController* logController, QSqlDatabase db);

    Q_INVOKABLE void setTypeFilter(int filter);

    Q_INVOKABLE void setSourceFilter(QString source);

protected:
    LogController* m_logController;

    QString m_sourceFilter;
    int m_typeFilter;

    void updateFilter();

    QVariant resolveData(int colIndex, QVariant value) const;

    void _setQuery();
};

#endif      // LOGSQLLISTMODEL_H
