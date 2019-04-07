#include "include/settingscontroller.h"
#include "include/constants_qt.h"
#include <QDebug>

SettingsController::SettingsController(QObject *parent) : ControllerBase(parent)
{

}

QString SettingsController::getName() {
    return SettingsController::staticMetaObject.className();
}

QStringList SettingsController::getTopicPath() {
    return QStringList() << MQTT_PATH_SETTINGS;
}

QStringList SettingsController::getLabelList() {
    CONVERT_LABEL_LIST(SETTINGS_LABELS);
}

QVariant::Type SettingsController::getValueType(int index) {
    MQTT_SETTINGS sett = static_cast<MQTT_SETTINGS>(index);

    switch(sett) {
    case SETTINGS_PREHEAT_FROM:
    case SETTINGS_PREHEAT_TO:
        return QVariant::Int;
    case SETTINGS_PREHEAT_HC_TEMP:
    case SETTINGS_PREHEAT_WATER_TEMP:
        return QVariant::Double;
    }

    return QVariant::Invalid;
}

void SettingsController::onInit() {
}

void SettingsController::onValueChanged(int index, QVariant value) {
    qWarning() << Q_FUNC_INFO << index << value;
}
