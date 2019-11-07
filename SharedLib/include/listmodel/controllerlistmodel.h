#ifndef CONTROLLERLISTMODEL_H
#define CONTROLLERLISTMODEL_H

#include <QAbstractListModel>
#include "../controller/controllerbase.h"

class ControllerListModel : public QAbstractListModel
{
public:
    ControllerListModel(ControllerBase* controller);

    enum ListRoles {
        LabelRole = Qt::UserRole + 1,
        ValueRole,
        IsValidRole,
        TrendRole
    };

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const;

private slots:
    void onValueChanged(int index, QVariant value);
    void onValueValidChanged(int index);
    void onValueTrendChanged(int index);

protected:
    ControllerBase* m_controller;

};

Q_DECLARE_METATYPE( ControllerListModel* );

#endif // CONTROLLERLISTMODEL_H
