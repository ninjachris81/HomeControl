#include "include/controller/tempcontroller.h"
#include "include/controller/settingscontroller.h"
#include "include/controller/controllermanager.h"

#include "include/controller/humiditycontroller.h"

#include "include/constants_qt.h"
#include "include/config/rpi_gpio_zero.h"

QString TempController::CONTROLLER_NAME = QStringLiteral("TempController");
Q_LOGGING_CATEGORY(LG_TEMP_CONTROLLER, "TempController");

TempController::TempController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
{

}

QString TempController::getName() {
    return CONTROLLER_NAME;
}

QStringList TempController::getTopicPath() {
    return QStringList() << MQTT_PATH_TEMPS;
}

QStringList TempController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::TEMPS_LABELS);
}

QString TempController::getEnumName() {
    return "MQTT_TEMPS";
}

QVariant::Type TempController::getDefaultValueType() {
    return QVariant::Double;
}

qint64 TempController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::TEMPS_HC:
    case EnumsDeclarations::TEMPS_WATER:
    case EnumsDeclarations::TEMPS_INSIDE:
    case EnumsDeclarations::TEMPS_SOLAR_HC:
        return LIFETIME_MID;
    case EnumsDeclarations::TEMPS_TANK:
        return LIFETIME_LONG;
    case EnumsDeclarations::TEMPS_OUTSIDE:
        return LIFETIME_LONG;
    case EnumsDeclarations::TEMPS_OUTSIDE2:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

qint64 TempController::getValueTrendLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::TEMPS_HC:
    case EnumsDeclarations::TEMPS_TANK:
    case EnumsDeclarations::TEMPS_WATER:
    case EnumsDeclarations::TEMPS_INSIDE:
    case EnumsDeclarations::TEMPS_SOLAR_HC:
        return VALUE_TT_MID;
    case EnumsDeclarations::TEMPS_OUTSIDE:
        return VALUE_TT_SLOW;
    case EnumsDeclarations::TEMPS_OUTSIDE2:
        return VALUE_TT_MID;
    default:
        return VALUE_TT_NONE;
    }
}

void TempController::onInit() {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO;
}

bool TempController::isValueOwner(int index) {
    switch (index) {
    case EnumsDeclarations::TEMPS_OUTSIDE2:
        return m_parent->deviceId()==DEV_ID_ZERO;
    default:
        return ControllerBase::isValueOwner(index);
    }
}

void TempController::onMqttConnected() {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO;
}

QVariant TempController::value(int index) {
    switch (index) {
    case EnumsDeclarations::TEMPS_TANK:
        return ControllerBase::value(index).toInt() + m_parent->getController(SettingsController::CONTROLLER_NAME)->value(EnumsDeclarations::SETTINGS_TANK_OFFSET).toDouble();
    default:
        return ControllerBase::value(index);
    }
}

void TempController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO << index << value;

    /*
    switch(index) {
    case MQTT_PATH_TEMPS_HC:
        break;
    }
    */
}

/*
void TempController::onScheduleUpdate() {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO;

    float t = 0.0f;
    float h = 0.0f;

    int res = dht.readDHT(DhtUtils::AM2302, DHT_TEMP_GPIO, &h, &t);

    if (res==DHT_SUCCESS) {
        setValue(MQTT_PATH_TEMPS_OUTSIDE2, t);
        publishValue(MQTT_PATH_TEMPS_OUTSIDE2);
    } else {
        qCWarning(LG_TEMP_CONTROLLER) << "Invalid reading from DHT" << res;
    }

    // a bit inconsistent, but also publish for humidity
    m_parent->getController(HumidityController::CONTROLLER_NAME)->setValue(MQTT_PATH_HUMIDITIES_OUTSIDE2, h);
    m_parent->getController(HumidityController::CONTROLLER_NAME)->publishValue(MQTT_PATH_HUMIDITIES_OUTSIDE2);
}
*/
