#include "include/settingscontroller.h"
#include "include/constants_qt.h"
#include <QDebug>

QString SettingsController::CONTROLLER_NAME = QStringLiteral("SettingsController");

SettingsController::SettingsController(SETTINGS_MODE mode, QObject *parent) : ControllerBase(parent), m_mode(mode)
{

}

QString SettingsController::getName() {
    return CONTROLLER_NAME;
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
    if (m_mode==SETTINGS_SERVER) {
        m_settings = new QSettings("settings.ini", QSettings::IniFormat);

        setValue(SETTINGS_PREHEAT_FROM, getSettingsValue(SETTINGS_PREHEAT_FROM, 7));
        setValue(SETTINGS_PREHEAT_TO, getSettingsValue(SETTINGS_PREHEAT_TO, 9));
        setValue(SETTINGS_PREHEAT_HC_TEMP, getSettingsValue(SETTINGS_PREHEAT_HC_TEMP, 26.0));
        setValue(SETTINGS_PREHEAT_WATER_TEMP, getSettingsValue(SETTINGS_PREHEAT_WATER_TEMP, 24.0));
    }
}

void SettingsController::onValueChanged(int index, QVariant value) {
    qWarning() << Q_FUNC_INFO << index << value;

    if (m_mode==SETTINGS_SERVER) {
        m_settings->setValue(getSettingsKey(index), value);
    }
}

QString SettingsController::getSettingsKey(int index) {
    QString key = getLabelList().at(index);
    return key.replace(" ", "_");
}

QVariant SettingsController::getSettingsValue(int index, QVariant defaultValue, bool createIfNotExists) {
    QString key = getSettingsKey(index);

    QVariant val = m_settings->value(key, defaultValue);
    if (val.canConvert(getValueType(index))) {
        val.convert(getValueType(index));

        if (createIfNotExists) {
            m_settings->setValue(key, val);
        }
        return val;
    } else {
        qWarning() << "Cannot convert value" << val << getValueType(index);
        return m_values[index];
    }
}

void SettingsController::onConnectedChanged(bool connected) {
    qDebug() << Q_FUNC_INFO << connected;

    if (m_mode==SETTINGS_SERVER) {
        if (connected) {
            broadcastValues();
        }
    }
}
