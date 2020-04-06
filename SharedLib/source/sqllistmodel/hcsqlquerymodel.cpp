#include "include/sqllistmodel/hcsqlquerymodel.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QSqlError>

HCSqlQueryModel::HCSqlQueryModel(QSqlDatabase db, QObject *parent) :
    QSqlTableModel(parent, db)
{
}

void HCSqlQueryModel::updateTable(const QString &tableName, const QString filter, const int sortCol, const Qt::SortOrder sortOrder) {
    qDebug() << Q_FUNC_INFO << tableName;

    if (database().tables().contains(tableName)) {
        setTable(tableName);
        setFilter(filter);
        generateRoleNames();
        setSort(sortCol, sortOrder);
        select();

        qDebug() << roleNames();
    } else {
        qWarning() << "Table" << tableName << "does not exist";
        qDebug() << database();
    }
}

void HCSqlQueryModel::generateRoleNames()
{
    qDebug() << Q_FUNC_INFO << rowCount();

    m_roleNames.clear();
    for( int i = 0; i < record().count(); i ++) {
        m_roleNames.insert(Qt::UserRole + i + 1, QVariant(this->headerData(i, Qt::Horizontal).toString().prepend("col_")).toByteArray());
    }
}

QVariant HCSqlQueryModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << Q_FUNC_INFO << index << role;
    QVariant value;

    if(role == Qt::DisplayRole) {
        value = resolveDisplayData(index, role, QSqlQueryModel::data(index, Qt::DisplayRole));
    } else if(role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    }
    else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = resolveData(columnIdx, QSqlQueryModel::data(modelIndex, Qt::DisplayRole));
    }
    return value;
}

QVariant HCSqlQueryModel::resolveData(int colIndex, QVariant value) const {
    Q_UNUSED(colIndex);

    return value;
}

QVariant HCSqlQueryModel::resolveDisplayData(const QModelIndex &item, int role, QVariant value) const {
    Q_UNUSED(value);

    return QSqlQueryModel::data(item, role);
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
