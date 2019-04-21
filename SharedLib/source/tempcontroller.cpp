#include "include/tempcontroller.h"
#include "include/constants_qt.h"

QString TempController::CONTROLLER_NAME = QStringLiteral("TempController");

TempController::TempController(QObject *parent) : ControllerBase(parent)
{

}

QString TempController::getName() {
    return CONTROLLER_NAME;
}

QStringList TempController::getTopicPath() {
    return QStringList() << MQTT_PATH_TEMPS;
}

QStringList TempController::getLabelList() {
    CONVERT_LABEL_LIST(TEMPS_LABELS);
}

QVariant::Type TempController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::Double;
}

void TempController::onInit() {
    qDebug() << Q_FUNC_INFO;
}

void TempController::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void TempController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    switch(index) {
    case MQTT_PATH_TEMPS_HC:
        break;
    }
}
