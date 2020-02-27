#include "include/wrapper/settingswrapper.h"

SettingsWrapper::SettingsWrapper(SettingsController* controller) : ControllerWrapper (controller) {
}

void SettingsWrapper::updateSetting(int index, QVariant newValue) {
    qDebug() << Q_FUNC_INFO << index << newValue;

    m_controller->setValue(index, newValue, true);
}
