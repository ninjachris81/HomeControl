#include "include/controller/tempcontroller.h"
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
    CONVERT_LABEL_LIST(EnumsDeclarations::TEMPS_LABELS);
}

QString TempController::getEnumName() {
    return "MQTT_TEMPS";
}

QVariant::Type TempController::getDefaultValueType() {
    return QVariant::Double;
}

qint64 TempController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::TEMPS_HC:
    case EnumsDeclarations::TEMPS_TANK:
    case EnumsDeclarations::TEMPS_WATER:
    case EnumsDeclarations::TEMPS_INSIDE:
    case EnumsDeclarations::TEMPS_SOLAR_HC:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

bool TempController::isValueOwner(int index) {
    Q_UNUSED(index);

    return false;
}

void TempController::onInit() {
    qDebug() << Q_FUNC_INFO;
}

void TempController::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void TempController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    /*
    switch(index) {
    case MQTT_PATH_TEMPS_HC:
        break;
    }
    */
}
