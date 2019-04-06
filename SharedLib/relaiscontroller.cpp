#include "include/relaiscontroller.h"
#include "constants.h"

RelaisController::RelaisController(QObject *parent) : ControllerBase (parent)
{

}

QString RelaisController::getName() {
    return "RelaisController";
}

QStringList RelaisController::getTopicPath() {
    return QStringList() << MQTT_PATH_RELAYS;
}

QStringList RelaisController::getLabelList() {
    return QStringList() << RELAYS_LABEL_HEAT_PUMP;
}

QVariant::Type RelaisController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::Bool;
}

void RelaisController::onInit() {
    m_values.append(false);     // MQTT_PATH_RELAYS_HEAT_PUMP
}

void RelaisController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}
