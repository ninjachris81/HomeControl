#ifndef QSQLDATABASEWRAPPER_H
#define QSQLDATABASEWRAPPER_H

#include <QObject>
#include <QSqlDatabase>

class QSqlDatabaseWrapper : public QObject {
    Q_OBJECT

public:
    QSqlDatabaseWrapper();

    bool open();

    void setDatabaseName(const QString& name);

    void addDatabase(const QString &type, const QString &connectionName);

    QSqlDatabase *db();

protected:
    QSqlDatabase m_db;

signals:
    void opened();

};

#endif          // QSQLDATABASEWRAPPER_H
