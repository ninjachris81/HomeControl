#include "include/controllerlistmodel.h"
#include <QQmlEngine>

ControllerListModel::ControllerListModel(ControllerBase *controller) : m_controller(controller)
{
    connect(controller, &ControllerBase::valueChanged, this, &ControllerListModel::onValueChanged);
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
    return m_controller->getValues().count();
}

QVariant ControllerListModel::data(const QModelIndex &index, int role) const {
    if (index.isValid()) {
        if (role==ValueRole) {
            return m_controller->getValues().at(index.row());
        } else if (role==LabelRole) {
            return m_controller->getLabel(index.row());
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
    return roles;
}
