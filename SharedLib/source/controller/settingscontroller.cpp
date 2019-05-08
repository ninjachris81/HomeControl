#include "include/controller/settingscontroller.h"
#include "include/constants_qt.h"
#include <QDebug>
#include <QHostInfo>
#include <QHostAddress>

QString SettingsController::CONTROLLER_NAME = QStringLiteral("SettingsController");

SettingsController::SettingsController(QObject *parent) : ControllerBase(parent)
{

}

QString SettingsController::getName() {
    return CONTROLLER_NAME;
}

QStringList SettingsController::getTopicPath() {
    return QStringList() << MQTT_PATH_SETTINGS;
}

QStringList SettingsController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::SETTINGS_LABELS);
}

QVariant::Type SettingsController::getValueType(int index) {
    EnumsDeclarations::MQTT_SETTINGS sett = static_cast<EnumsDeclarations::MQTT_SETTINGS>(index);

    switch(sett) {
    case EnumsDeclarations::SETTINGS_PREHEAT_FROM:
    case EnumsDeclarations::SETTINGS_PREHEAT_TO:
        return QVariant::Int;
    case EnumsDeclarations::SETTINGS_PREHEAT_HC_TEMP:
    case EnumsDeclarations::SETTINGS_PREHEAT_WATER_TEMP:
        return QVariant::Double;
    case EnumsDeclarations::SETTINGS_PREHEAT_MODE:
    case EnumsDeclarations::SETTINGS_PREHEAT_DURATION:
        return QVariant::Int;
    case EnumsDeclarations::SETTINGS_HEATING_TEMP:
        return QVariant::Double;
    case EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE:
        return QVariant::Bool;
    case EnumsDeclarations::SETTINGS_HEATING_MODE:
        return QVariant::Int;
    case EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE:
        return QVariant::Bool;
    case EnumsDeclarations::SETTINGS_CORE_HOST:
        return QVariant::String;
    }

    return QVariant::Invalid;
}

qint64 SettingsController::getValueLifetime(int index) {
    Q_UNUSED(index);

    return LIFETIME_UNLIMITED;
}

void SettingsController::onInit() {
    if (m_mode==VALUE_OWNER_SERVER) {
        m_settings = new QSettings("settings.ini", QSettings::IniFormat);

        setValue(EnumsDeclarations::SETTINGS_PREHEAT_FROM, getSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_FROM, 7));
        setValue(EnumsDeclarations::SETTINGS_PREHEAT_TO, getSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_TO, 9));
        setValue(EnumsDeclarations::SETTINGS_PREHEAT_HC_TEMP, getSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_HC_TEMP, 26.0));
        setValue(EnumsDeclarations::SETTINGS_PREHEAT_WATER_TEMP, getSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_WATER_TEMP, 24.0));
        setValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, getSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC));
        setValue(EnumsDeclarations::SETTINGS_PREHEAT_DURATION, getSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_DURATION, 600));
        setValue(EnumsDeclarations::SETTINGS_HEATING_TEMP, getSettingsValue(EnumsDeclarations::SETTINGS_HEATING_TEMP, 22));
        setValue(EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE, getSettingsValue(EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE, true));
        setValue(EnumsDeclarations::SETTINGS_HEATING_MODE, getSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC));
        setValue(EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE, getSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE, false));

        // publish hostname
        setValue(EnumsDeclarations::SETTINGS_CORE_HOST, QHostInfo::localHostName());
    }
}

void SettingsController::onValueChanged(int index, QVariant value) {
    qWarning() << Q_FUNC_INFO << index << value;

    if (m_mode==VALUE_OWNER_SERVER) {
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
        return m_values[index].value;
    }
}

void SettingsController::onConnectedChanged(bool connected) {
    qDebug() << Q_FUNC_INFO << connected;

    if (m_mode==VALUE_OWNER_SERVER) {
        if (connected) {
            broadcastValues();
        }
    }
}
