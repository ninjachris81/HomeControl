#include "include/controller/brightnesscontroller.h"
#include "include/constants_qt.h"
#include "include/controller/controllermanager.h"

QString BrightnessController::CONTROLLER_NAME = QStringLiteral("BrightnessController");
Q_LOGGING_CATEGORY(LG_BRIGHTNESS_CONTROLLER, "BrightnessController");

BrightnessController::BrightnessController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
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

void BrightnessController::onInit() {
    qCDebug(LG_BRIGHTNESS_CONTROLLER) << Q_FUNC_INFO;

    if (m_parent->deviceId()==DEV_ID_ZERO) {
        analogReader.configure(RPI_BRIGHTNESS_ADC, RPI_BRIGHTNESS_GAIN);
        startScheduler(BRIGHTNESS_UPDATE_VALUE_INTERVAL);
    }
}

bool BrightnessController::isValueOwner(int index) {
    switch (index) {
    case EnumsDeclarations::BRIGHTNESSES_OUTSIDE:
        return m_parent->deviceId()==DEV_ID_ZERO;
    default:
        return ControllerBase::isValueOwner(index);
    }
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

void BrightnessController::onScheduleUpdate() {
    if (m_parent->deviceId()==DEV_ID_ZERO) {
        int brightnessRaw = analogReader.readValue(RPI_BRIGHTNESS_ADC);

        qCDebug(LG_BRIGHTNESS_CONTROLLER) << "Brightness Raw:" << brightnessRaw;
        setValue(EnumsDeclarations::BRIGHTNESSES_OUTSIDE, brightnessRaw);
        publishValue(EnumsDeclarations::BRIGHTNESSES_OUTSIDE);
    }
}
