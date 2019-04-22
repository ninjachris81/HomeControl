#include "include/controller/relaycontroller.h"
#include "include/constants_qt.h"

QString RelayController::CONTROLLER_NAME = QStringLiteral("RelayController");

RelayController::RelayController(QObject *parent) : ControllerBase (parent)
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

QVariant::Type RelayController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::Bool;
}

bool RelayController::isValueOwner(int index) {
    Q_UNUSED(index);

    return false;
}

qint64 RelayController::getValueLifetime(int index) {
    switch(index) {
    case EnumsDeclarations::RELAYS_HC_PUMP:
    case EnumsDeclarations::RELAYS_WATER_PUMP:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

void RelayController::onInit() {
}

void RelayController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}
