#include "include/relaiscontroller.h"
#include "include/constants_qt.h"

RelaisController::RelaisController(QObject *parent) : ControllerBase (parent)
{

}

QString RelaisController::getName() {
    return RelaisController::staticMetaObject.className();
}

QStringList RelaisController::getTopicPath() {
    return QStringList() << MQTT_PATH_RELAYS;
}

QStringList RelaisController::getLabelList() {
    CONVERT_LABEL_LIST(RELAYS_LABELS);
}

QVariant::Type RelaisController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::Bool;
}

void RelaisController::onInit() {
}

void RelaisController::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}
