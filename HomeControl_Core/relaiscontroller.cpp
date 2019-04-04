#include "relaiscontroller.h"
#include "constants.h"

RelaisController::RelaisController(AppConfiguration *appConfig, QObject *parent) : ControllerBase (appConfig, parent)
{

}

QString RelaisController::getName() {
    return "RelaisController";
}

QStringList RelaisController::getTopicPath() {
    return QStringList() << MQTT_PATH_RELAYS;
}

void RelaisController::onInit() {
    m_values.append(false);     // MQTT_PATH_RELAYS_HEAT_PUMP
}

void RelaisController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}
