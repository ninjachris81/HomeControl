#ifndef LOGSQLLISTMODEL_H
#define LOGSQLLISTMODEL_H

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>

#include "sqlquerymodel.h"
#include "../controller/logcontroller.h"

class LogSqlListModel : public SqlQueryModel {
Q_OBJECT

public:
    explicit LogSqlListModel(QObject *parent, QSqlDatabase db);

protected:
    LogController* m_logController;

    void _setQuery();
};

#endif      // LOGSQLLISTMODEL_H
