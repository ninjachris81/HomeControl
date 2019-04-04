#include "errorcontroller.h"
#include "constants.h"
#include <QDebug>

ErrorController::ErrorController(AppConfiguration *appConfig, QObject *parent) : ControllerBase(appConfig, parent)
{

}

QString ErrorController::getName() {
    return "ErrorController";
}

QStringList ErrorController::getTopicPath() {
    return QStringList() << MQTT_PATH_ERRORS;
}

void ErrorController::onInit() {
    m_values.append("");        // MQTT_PATH_ERRORS_RELAYS
}

void ErrorController::onValueChanged(int index, QVariant value) {
    qWarning() << Q_FUNC_INFO << index << value;
}
