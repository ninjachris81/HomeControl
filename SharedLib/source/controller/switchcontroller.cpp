#include "include/controller/switchcontroller.h"
#include "include/constants_qt.h"

#include "include/controller/controllermanager.h"

#include "include/config/rpi_gpio_zero.h"

QString SwitchController::CONTROLLER_NAME = QStringLiteral("SwitchController");
Q_LOGGING_CATEGORY(LG_SWITCH_CONTROLLER, "SwitchController");

SwitchController::SwitchController(QObject *parent) : ControllerBase (ControllerBase::VALUE_OWNER_EXTERNAL_SENSOR, parent)
{

}

QString SwitchController::getName() {
    return CONTROLLER_NAME;
}

QStringList SwitchController::getTopicPath() {
    return QStringList() << MQTT_PATH_SWITCHES;
}

QStringList SwitchController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::SWITCHES_LABELS);
}

QVariant::Type SwitchController::getDefaultValueType() {
    return QVariant::Bool;
}

QString SwitchController::getEnumName() {
    return "MQTT_SWITCHES";
}

SwitchController::VALUE_BC_INTERVAL SwitchController::getValueBCInterval(int index) {
    Q_UNUSED(index)
    return VALUE_BC_SLOW;
}

qint64 SwitchController::getValueLifetime(int index) {
    switch(index) {
    case EnumsDeclarations::SWITCHES_WINTERGARDEN:
        return LIFETIME_LONG;
    default:
        return LIFETIME_UNLIMITED;
    }
}

void SwitchController::onInit() {
    if (m_parent->deviceId()==DEV_ID_ZERO) {
        gpioManager.configureAsInput(PIR_SENSOR_GPIO);
        startScheduler(SWITCH_UPDATE_VALUE_INTERVAL);
    }
}

void SwitchController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}

void SwitchController::onScheduleUpdate() {
    if (m_parent->deviceId()==DEV_ID_ZERO) {
        bool pirOn = gpioManager.read(PIR_SENSOR_GPIO);

        qCDebug(LG_SWITCH_CONTROLLER) << "PIR:" << pirOn;
        setValue(MQTT_PATH_CURRENTS_PV, pirOn, true);
    }
}
