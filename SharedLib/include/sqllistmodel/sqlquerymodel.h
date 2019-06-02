#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlDatabase>
#include <QSqlTableModel>

class SqlQueryModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit SqlQueryModel(QSqlDatabase db, QObject *parent=nullptr);

    void updateTable(const QString &tableName);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const {	return m_roleNames;	}

protected:
    virtual QVariant resolveData(int colIndex, QVariant value) const;

private:
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
};

#endif // SQLQUERYMODEL_H