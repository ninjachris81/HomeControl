#include "include/controller/currentcontroller.h"
#include "include/constants_qt.h"

QString CurrentController::CONTROLLER_NAME = QStringLiteral("CurrentController");
Q_LOGGING_CATEGORY(LG_CURRENT_CONTROLLER, "CurrentController");

CurrentController::CurrentController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
{

}

QString CurrentController::getName() {
    return CONTROLLER_NAME;
}

QStringList CurrentController::getTopicPath() {
    return QStringList() << MQTT_PATH_CURRENTS;
}

QStringList CurrentController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::CURRENTS_LABELS);
}

QString CurrentController::getEnumName() {
    return "MQTT_CURRENTS";
}

QVariant::Type CurrentController::getDefaultValueType() {
    return QVariant::Double;
}

qint64 CurrentController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::CURRENTS_MAIN_BASEMENT:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

void CurrentController::onInit() {
    qCDebug(LG_CURRENT_CONTROLLER) << Q_FUNC_INFO;
}

void CurrentController::onMqttConnected() {
    qCDebug(LG_CURRENT_CONTROLLER) << Q_FUNC_INFO;
}

void CurrentController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_CURRENT_CONTROLLER) << Q_FUNC_INFO << index << value;

    switch(index) {
    case MQTT_PATH_CURRENTS_MAIN_BASEMENT:
        break;
    }
}
