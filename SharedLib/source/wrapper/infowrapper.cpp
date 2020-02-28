#include "include/wrapper/infowrapper.h"

InfoWrapper::InfoWrapper(InfoController *controller) : ControllerWrapper(controller)
{
    connect(static_cast<InfoController*>(controller), &InfoController::timeIsOffsetChanged, this, &InfoWrapper::onTimeIsOffsetChanged);
    connect(static_cast<InfoController*>(controller), &InfoController::valueChanged, this, &InfoWrapper::onValueChanged);
}

bool InfoWrapper::timeIsOffset() {
    return static_cast<InfoController*>(m_controller)->timeIsOffset();
}

void InfoWrapper::onTimeIsOffsetChanged(bool isOffset) {
    Q_UNUSED(isOffset);
    Q_EMIT(timeIsOffsetChanged());
}

void InfoWrapper::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    Q_UNUSED(value);

    switch (index) {
    case EnumsDeclarations::INFOS_SYSTEM_TIME:
        Q_EMIT(systemTimeChanged());
        break;
    case EnumsDeclarations::INFOS_SYSTEM_TEMP:
        Q_EMIT(systemTempChanged());
        break;
    case EnumsDeclarations::INFOS_BOILER_TARGET_TEMP:
        Q_EMIT(boilerTargetTempChanged());
        break;
    default:
        break;
    }
}

QDateTime InfoWrapper::systemTime() {
    return QDateTime::fromString(m_controller->value(EnumsDeclarations::INFOS_SYSTEM_TIME).toString(), Qt::ISODate);
}

double InfoWrapper::systemTemp() {
    return m_controller->value(EnumsDeclarations::INFOS_SYSTEM_TEMP).toDouble();
}

double InfoWrapper::boilerTargetTemp() {
    return m_controller->value(EnumsDeclarations::INFOS_BOILER_TARGET_TEMP).toDouble();
}
