#include "include/tempcontroller.h"
#include "constants.h"

TempController::TempController(QObject *parent) : ControllerBase(parent)
{

}

QString TempController::getName() {
    return "TempController";
}

QStringList TempController::getTopicPath() {
    return QStringList() << MQTT_PATH_TEMPERATURES;
}

QStringList TempController::getLabelList() {
    return QStringList() << TEMPERATURES_LABEL_INSIDE;
}

QVariant::Type TempController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::Double;
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
