#include "include/controller/settingscontroller.h"
#include "include/controller/controllermanager.h"
#include "include/controller/tempcontroller.h"
#include "include/constants_qt.h"
#include <QDebug>
#include <QHostInfo>
#include <QHostAddress>

QString SettingsController::CONTROLLER_NAME = QStringLiteral("SettingsController");
Q_LOGGING_CATEGORY(LG_SETTINGS_CONTROLLER, "SettingsController");

SettingsController::SettingsController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
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

QList<QVariant::Type> SettingsController::getValueTypes() {
    QList<QVariant::Type> returnList;

    for (QVariant::Type type : EnumsDeclarations::SETTINGS_TYPES) {
        returnList.append(type);
    }

    return returnList;
}

QString SettingsController::getEnumName() {
    return "MQTT_SETTINGS";
}

QVariant::Type SettingsController::getDefaultValueType() {
    return QVariant::String;
}

qint64 SettingsController::getValueLifetime(int index) {
    Q_UNUSED(index);

    return LIFETIME_UNLIMITED;
}

void SettingsController::onInit() {
    if (m_parent->isServer()) {
        m_settings = new QSettings("settings.ini", QSettings::IniFormat);

        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_FROM, 7);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_TO, 9);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_HC_TEMP, 26.0);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_WATER_TEMP, 24.0);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_DURATION, 600);
        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_TEMP, 22);
        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_USE_TOGGLE, true);
        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MODE, EnumsDeclarations::SETTING_MODE_AUTOMATIC);
        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MANUAL_STATE, false);

        // publish hostname
        setValue(EnumsDeclarations::SETTINGS_CORE_HOST, QHostInfo::localHostName());

        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_HC_STANDBY_TEMP, 26.0);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_STANDBY_FROM, 9);
        publishSettingsValue(EnumsDeclarations::SETTINGS_PREHEAT_STANDBY_TO, 21);

        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MONTH_FROM, 9);
        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MONTH_TO, 5);

        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_TOGGLE_ON_DURATION, 60000);

        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_HOUR_FROM, 6);
        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_HOUR_TO, 21);

        publishSettingsValue(EnumsDeclarations::SETTINGS_HEATING_MIN_TEMP_TANK, 30);

        publishSettingsValue(EnumsDeclarations::SETTINGS_TANK_OFFSET, 0);
        publishSettingsValue(EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD, 10);
        publishSettingsValue(EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD, 50);
        publishSettingsValue(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP, 40);
        publishSettingsValue(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE, "-15 -15 -15 -15 -5 0 5 5 5 5 -5 -5 -5 -5 -5 -5 0 0 0 0 0 -5 -15 -15");
        publishSettingsValue(EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT, 5);
    }
}

void SettingsController::publishSettingsValue(EnumsDeclarations::MQTT_SETTINGS key, QVariant defaultValue) {
    setValue(key, getSettingsValue(key, defaultValue));
}

void SettingsController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_SETTINGS_CONTROLLER) << Q_FUNC_INFO << index << value;

    if (m_parent->isServer()) {
        m_settings->setValue(getSettingsKey(index), value);
    } else {
        switch (index) {
        case EnumsDeclarations::SETTINGS_TANK_OFFSET:
            qCDebug(LG_SETTINGS_CONTROLLER) << Q_FUNC_INFO << "Emitting refresh signal";
            ControllerBase* tempController = m_parent->getController(TempController::CONTROLLER_NAME);
            Q_EMIT(tempController->valueChanged(EnumsDeclarations::TEMPS_TANK, tempController->value(EnumsDeclarations::TEMPS_TANK)));
            break;
        }
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
        qCWarning(LG_SETTINGS_CONTROLLER) << "Cannot convert value" << val << getValueType(index);
        return value(index);
    }
}

void SettingsController::onConnectedChanged(bool connected) {
    qCDebug(LG_SETTINGS_CONTROLLER) << Q_FUNC_INFO << connected;

    if (m_parent->isServer()) {
        if (connected) {
            broadcastValues();
        }
    }
}
