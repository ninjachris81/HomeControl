#include "include/controller/brightnesscontroller.h"
#include "include/constants_qt.h"

QString BrightnessController::CONTROLLER_NAME = QStringLiteral("BrightnessController");
Q_LOGGING_CATEGORY(LG_BRIGHTNESS_CONTROLLER, "BrightnessController");

BrightnessController::BrightnessController(QObject *parent) : ControllerBase(parent)
{

}

QString BrightnessController::getName() {
    return CONTROLLER_NAME;
}

QStringList BrightnessController::getTopicPath() {
    return QStringList() << MQTT_PATH_BRIGHTNESSES;
}

QStringList BrightnessController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::BRIGHTNESSES_LABELS);
}

QString BrightnessController::getEnumName() {
    return "MQTT_BRIGHTNESSES";
}

QVariant::Type BrightnessController::getDefaultValueType() {
    return QVariant::Int;
}

qint64 BrightnessController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::BRIGHTNESSES_SOLAR:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

bool BrightnessController::isValueOwner(int index) {
    Q_UNUSED(index);

    return false;
}

void BrightnessController::onInit() {
    qCDebug(LG_BRIGHTNESS_CONTROLLER) << Q_FUNC_INFO;
}

void BrightnessController::onMqttConnected() {
    qCDebug(LG_BRIGHTNESS_CONTROLLER) << Q_FUNC_INFO;
}

void BrightnessController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_BRIGHTNESS_CONTROLLER) << Q_FUNC_INFO << index << value;

    switch(index) {
    case MQTT_PATH_BRIGHTNESSES_SOLAR:
        break;
    }
}
