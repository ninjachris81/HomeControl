#ifndef LOGSQLLISTMODEL_H
#define LOGSQLLISTMODEL_H

#include <QSqlQueryModel>

class LogSqliListModel : public QSqlQueryModel {
Q_OBJECT

public:
    explicit LogSqliListModel(QObject *parent = 0);

};

#endif      // LOGSQLLISTMODEL_H
