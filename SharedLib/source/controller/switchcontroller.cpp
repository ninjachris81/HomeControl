#include "include/controller/switchcontroller.h"
#include "include/constants_qt.h"

QString SwitchController::CONTROLLER_NAME = QStringLiteral("SwitchController");

SwitchController::SwitchController(QObject *parent) : ControllerBase (parent)
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

bool SwitchController::isValueOwner(int index) {
    Q_UNUSED(index);

    return false;
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
}

void SwitchController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}
