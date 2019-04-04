#include "tempcontroller.h"
#include "constants.h"

TempController::TempController(AppConfiguration *appConfig, QObject *parent) : ControllerBase(appConfig, parent)
{

}

QString TempController::getName() {
    return "TempController";
}

QStringList TempController::getTopicPath() {
    return QStringList() << MQTT_PATH_TEMPERATURES;
}

void TempController::onInit() {
    qDebug() << Q_FUNC_INFO;

    m_values.append(0.0);       // MQTT_PATH_TEMPERATURES_INSIDE
}

void TempController::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void TempController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    switch(index) {
    case MQTT_PATH_TEMPERATURES_INSIDE:
        break;
    }
}
