#ifndef LOGSQLLISTMODEL_H
#define LOGSQLLISTMODEL_H

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>

#include "sqlquerymodel.h"
#include "controller/logcontroller.h"

class LogSqlListModel : public SqlQueryModel {
Q_OBJECT

public:
    explicit LogSqlListModel(LogController* logController, QSqlDatabase db);

    Q_INVOKABLE void setTypeFilter(int filter);

    Q_INVOKABLE void setSourceFilter(QString source);

protected slots:
    void onLogChanged();

protected:
    LogController* m_logController;

    QVariant resolveData(int colIndex, QVariant value) const;

    void _setQuery();
};

#endif      // LOGSQLLISTMODEL_H
