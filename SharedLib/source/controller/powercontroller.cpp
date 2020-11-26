#include "include/controller/powercontroller.h"
#include "include/constants_qt.h"

#include "include/controller/controllermanager.h"

QString PowerController::CONTROLLER_NAME = QStringLiteral("PowerController");
Q_LOGGING_CATEGORY(LG_POWER_CONTROLLER, "PowerController");

PowerController::PowerController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
{

}

QString PowerController::getName() {
    return CONTROLLER_NAME;
}

QStringList PowerController::getTopicPath() {
    return QStringList() << MQTT_PATH_POWERS;
}

QStringList PowerController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::POWERS_LABELS);
}

QString PowerController::getEnumName() {
    return "MQTT_POWERS";
}

QVariant::Type PowerController::getDefaultValueType() {
    return QVariant::Double;
}

qint64 PowerController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::POWERS_MAIN:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

void PowerController::onInit() {
    qCDebug(LG_POWER_CONTROLLER) << Q_FUNC_INFO;
}

void PowerController::onMqttConnected() {
    qCDebug(LG_POWER_CONTROLLER) << Q_FUNC_INFO;
}

void PowerController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_POWER_CONTROLLER) << Q_FUNC_INFO << index << value;

    switch(index) {
    case MQTT_PATH_POWERS_MAIN:
        break;
    }
}
