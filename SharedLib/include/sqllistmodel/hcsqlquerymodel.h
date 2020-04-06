#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTimer>

class HCSqlQueryModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit HCSqlQueryModel(QSqlDatabase db, QObject *parent=nullptr);

    void updateTable(const QString &tableName, const QString filter, const int sortCol, const Qt::SortOrder sortOrder);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const {	return m_roleNames;	}

    int limit() const{
        return m_limit;
    }
    void setLimit(int limit){
        if(limit > 0)
            m_limit = limit;
    }

protected:
    QTimer m_checkOpenTimer;

    virtual QVariant resolveDisplayData(const QModelIndex &item, int role, QVariant value) const;
    virtual QVariant resolveData(int colIndex, QVariant value) const;

    virtual void _setQuery() = 0;

    QString selectStatement() const
    {
        QString query = QSqlTableModel::selectStatement();
        if(m_limit > 0)
            query += QString(" LIMIT %1").arg(m_limit);
        return query;
    }

protected slots:
    void onCheckOpen();

private:
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
    int m_limit = -1;

signals:
    void databaseOpen();

};

#endif // SQLQUERYMODEL_H
