#include "include/errorcontroller.h"
#include "constants.h"
#include <QDebug>

ErrorController::ErrorController(QObject *parent) : ControllerBase(parent)
{
}

QString ErrorController::getName() {
    return "ErrorController";
}

QStringList ErrorController::getTopicPath() {
    return QStringList() << MQTT_PATH_ERRORS;
}

QStringList ErrorController::getLabelList() {
    return QStringList() << ERRORS_LABEL_RELAYS;
}

QVariant::Type ErrorController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::StringList;
}

void ErrorController::onInit() {
    m_values.append(QStringList());        // MQTT_PATH_ERRORS_RELAYS
}

void ErrorController::onValueChanged(int index, QVariant value) {
    qWarning() << Q_FUNC_INFO << index << value;
}
