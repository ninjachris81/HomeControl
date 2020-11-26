#include "include/controller/relaycontroller.h"
#include "include/constants_qt.h"
#include "include/controller/controllermanager.h"
#include "include/config/rpi_gpio_zero.h"

QString RelayController::CONTROLLER_NAME = QStringLiteral("RelayController");
Q_LOGGING_CATEGORY(LG_RELAY_CONTROLLER, "RelayController");

RelayController::RelayController(QObject *parent) : ControllerBase (ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
{

}

QString RelayController::getName() {
    return CONTROLLER_NAME;
}

QStringList RelayController::getTopicPath() {
    return QStringList() << MQTT_PATH_RELAYS;
}

QStringList RelayController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::RELAYS_LABELS);
}

QVariant::Type RelayController::getDefaultValueType() {
    return QVariant::Bool;
}

QString RelayController::getEnumName() {
    return "MQTT_RELAYS";
}

qint64 RelayController::getValueLifetime(int index) {
    switch(index) {
    case EnumsDeclarations::RELAYS_HC_PUMP:
    case EnumsDeclarations::RELAYS_WATER_PUMP:
    case EnumsDeclarations::RELAYS_HEATING_PUMP:
    case EnumsDeclarations::RELAYS_BOILER:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

void RelayController::onInit() {
    if (m_parent->deviceId()==DEV_ID_ZERO) {
        gpioManager.configureAsOutput(LIGHT_RELAY_GPIO);
    }
}

bool RelayController::isValueOwner(int index) {
    switch (index) {
    case EnumsDeclarations::RELAYS_LIGHT_OUTSIDE:
        return m_parent->deviceId()==DEV_ID_ZERO;
    default:
        return ControllerBase::isValueOwner(index);
    }
}

void RelayController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_RELAY_CONTROLLER) << Q_FUNC_INFO << index << value;

    switch(index) {
    case EnumsDeclarations::RELAYS_LIGHT_OUTSIDE:
        if (m_parent->deviceId()==DEV_ID_ZERO) {
            gpioManager.write(LIGHT_RELAY_GPIO, value.toBool());
        }
    }
}
