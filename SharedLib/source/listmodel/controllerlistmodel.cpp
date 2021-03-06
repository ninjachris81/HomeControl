#include "include/listmodel/controllerlistmodel.h"
#include <QQmlEngine>

ControllerListModel::ControllerListModel(ControllerBase *controller) : m_controller(controller)
{
    connect(controller, &ControllerBase::valueChanged, this, &ControllerListModel::onValueChanged);
    connect(controller, &ControllerBase::valueValidChanged, this, &ControllerListModel::onValueValidChanged);
    connect(controller, &ControllerBase::valueTrendChanged, this, &ControllerListModel::onValueTrendChanged);
}

void ControllerListModel::onValueValidChanged(int index) {
    QModelIndex itemIndex = this->index(index);

    Q_EMIT(dataChanged(itemIndex, itemIndex));
}

void ControllerListModel::onValueTrendChanged(int index) {
    qDebug() << Q_FUNC_INFO << index;

    QModelIndex itemIndex = this->index(index);

    Q_EMIT(dataChanged(itemIndex, itemIndex));
}

void ControllerListModel::onValueChanged(int index, QVariant value) {
    Q_UNUSED(value)

    QModelIndex itemIndex = this->index(index);

    Q_EMIT(dataChanged(itemIndex, itemIndex));
}

int ControllerListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 1;
}

int ControllerListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_controller->values().count();
}

QVariant ControllerListModel::data(const QModelIndex &index, int role) const {
    if (index.isValid()) {
        if (role==ValueRole) {
            return m_controller->value(index.row());
        } else if (role==LabelRole) {
            return m_controller->getLabel(index.row());
        } else if (role==IsValidRole) {
            return m_controller->values()[index.row()].isValid();
        } else if (role==TrendRole) {
            return m_controller->values()[index.row()].valueTrend();
        } else {
            return QVariant();
        }
    } else {
        return QVariant();
    }
}

QHash<int, QByteArray> ControllerListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[LabelRole] = "label";
    roles[ValueRole] = "value";
    roles[TrendRole] = "trend";
    return roles;
}

