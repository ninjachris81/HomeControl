#include "heatingcontroller.h"
#include "constants.h"

HeatingController::HeatingController(AppConfiguration *appConfig, QObject *parent) : ControllerBase(appConfig, parent)
{

}

QString HeatingController::getName() {
    return "HeatingController";
}

QMqttTopicFilter HeatingController::getTopicFilter() {
    return QMqttTopicFilter(MQTT_PATH_INSIDE_TEMP_1);
}

void HeatingController::onInit() {
    qDebug() << Q_FUNC_INFO;

    m_valueMap.insert(MQTT_PATH_INSIDE_TEMP_1, 0.0);
}

void HeatingController::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void HeatingController::onValueChanged(QString topic, QVariant value) {
    qDebug() << Q_FUNC_INFO << topic << value;

    if (topic==MQTT_PATH_INSIDE_TEMP_1) {

    }
}
