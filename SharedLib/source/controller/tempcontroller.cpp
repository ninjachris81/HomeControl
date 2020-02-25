#include "include/controller/tempcontroller.h"
#include "include/constants_qt.h"

QString TempController::CONTROLLER_NAME = QStringLiteral("TempController");
Q_LOGGING_CATEGORY(LG_TEMP_CONTROLLER, "TempController");

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
    case EnumsDeclarations::TEMPS_WATER:
    case EnumsDeclarations::TEMPS_INSIDE:
    case EnumsDeclarations::TEMPS_SOLAR_HC:
        return LIFETIME_MID;
    case EnumsDeclarations::TEMPS_TANK:
        return LIFETIME_LONG;
    default:
        return LIFETIME_UNLIMITED;
    }
}

qint64 TempController::getValueTrendLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::TEMPS_HC:
    case EnumsDeclarations::TEMPS_TANK:
    case EnumsDeclarations::TEMPS_WATER:
    case EnumsDeclarations::TEMPS_INSIDE:
    case EnumsDeclarations::TEMPS_SOLAR_HC:
        return VALUE_TT_MID;
    default:
        return VALUE_TT_NONE;
    }
}

bool TempController::isValueOwner(int index) {
    Q_UNUSED(index);

    return false;
}

void TempController::onInit() {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO;
}

void TempController::onMqttConnected() {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO;
}

void TempController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_TEMP_CONTROLLER) << Q_FUNC_INFO << index << value;

    /*
    switch(index) {
    case MQTT_PATH_TEMPS_HC:
        break;
    }
    */
}
