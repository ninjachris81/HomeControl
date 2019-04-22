#include "include/wrapper/controllerwrapper.h"
#include <QDebug>

ControllerWrapper::ControllerWrapper(ControllerBase *controller) : m_controller(controller) {
}

void ControllerWrapper::setValue(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (value.convert(m_controller->getValueType(index))) {
        if (m_controller->isValueOwner(index)) {
            qDebug() << "Is value owner";
            m_controller->setValue(index, value, false);
        } else {
            qDebug() << "Is not value owner - delegating";
            m_controller->setValue(index, value, true);
        }
    } else {
        qWarning() << "Unable to convert type";
    }
}

void ControllerWrapper::sendCmd(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;

    m_controller->publishCmd(cmd);
}
