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
    Q_UNUSED(value);

    switch (index) {
    case EnumsDeclarations::INFOS_SYSTEM_TIME:
        Q_EMIT(serverTimeChanged());
        break;
    default:
        break;
    }
}

QDateTime InfoWrapper::serverTime() {
    return QDateTime::fromString(m_controller->value(EnumsDeclarations::INFOS_SYSTEM_TIME).toString(), Qt::ISODate);
}
