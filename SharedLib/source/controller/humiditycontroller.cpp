#include "include/controller/humiditycontroller.h"
#include "include/controller/controllermanager.h"
#include "include/constants_qt.h"

QString HumidityController::CONTROLLER_NAME = QStringLiteral("HumidityController");
Q_LOGGING_CATEGORY(LG_HUMIDITY_CONTROLLER, "HumidityController");

HumidityController::HumidityController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
{

}

QString HumidityController::getName() {
    return CONTROLLER_NAME;
}

QStringList HumidityController::getTopicPath() {
    return QStringList() << MQTT_PATH_HUMIDITIES;
}

QStringList HumidityController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::HUMIDITIES_LABELS);
}

QString HumidityController::getEnumName() {
    return "MQTT_HUMIDITIES";
}

QVariant::Type HumidityController::getDefaultValueType() {
    return QVariant::Int;
}

qint64 HumidityController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::HUMIDITIES_OUTSIDE:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

void HumidityController::onInit() {
    qCDebug(LG_HUMIDITY_CONTROLLER) << Q_FUNC_INFO;
}

bool HumidityController::isValueOwner(int index) {
    switch (index) {
    case EnumsDeclarations::HUMIDITIES_OUTSIDE2:         // even though this is managed in TempController
        return m_parent->deviceId()==DEV_ID_ZERO;
    default:
        return ControllerBase::isValueOwner(index);
    }
}

void HumidityController::onMqttConnected() {
    qCDebug(LG_HUMIDITY_CONTROLLER) << Q_FUNC_INFO;
}

void HumidityController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_HUMIDITY_CONTROLLER) << Q_FUNC_INFO << index << value;

    switch(index) {
    case EnumsDeclarations::HUMIDITIES_OUTSIDE2:
        break;
    }
}
