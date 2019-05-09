#include "include/sqllistmodel/sqlquerymodel.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QSqlError>
#include <QModelIndex>

SqlQueryModel::SqlQueryModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent, db)
{
}

void SqlQueryModel::updateTable(const QString &tableName) {
    qDebug() << Q_FUNC_INFO << tableName;

    if (database().tables().contains(tableName)) {
        setTable(tableName);
        setFilter("");
        generateRoleNames();
        select();

        qDebug() << roleNames();
    } else {
        qWarning() << "Table" << tableName << "does not exist";
        qDebug() << database();
    }
}

void SqlQueryModel::generateRoleNames()
{
    qDebug() << Q_FUNC_INFO << rowCount();

    m_roleNames.clear();
    for( int i = 0; i < record().count(); i ++) {
        m_roleNames.insert(Qt::UserRole + i + 1, QVariant(this->headerData(i, Qt::Horizontal).toString().prepend("col_")).toByteArray());
    }
}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if(role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    }
    else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

/*
QVariant SqlQueryModel::data ( const QModelIndex & index, int role ) const
{
    qDebug() << index << role;

    if(index.row() >= rowCount())
    {
        return QString("");
    }

    if(role < Qt::UserRole) {
        return QSqlQueryModel::data(index, role);
    } else {
        // if no valid relationship was found
        return QSqlQueryModel::data(this->index(index.row(), role - Qt::UserRole - 1), Qt::DisplayRole);
    }
}*/
