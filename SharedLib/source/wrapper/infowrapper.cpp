#include "include/wrapper/infowrapper.h"

InfoWrapper::InfoWrapper(InfoController *controller) : ControllerWrapper(controller)
{
    connect(static_cast<InfoController*>(controller), &InfoController::timeIsOffsetChanged, this, &InfoWrapper::onTimeIsOffsetChanged);
}

bool InfoWrapper::timeIsOffset() {
    return static_cast<InfoController*>(m_controller)->timeIsOffset();
}

void InfoWrapper::onTimeIsOffsetChanged(bool isOffset) {
    Q_UNUSED(isOffset);
    Q_EMIT(timeIsOffsetChanged());
}
