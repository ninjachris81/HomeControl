#include "include/errorcontroller.h"
#include "include/constants_qt.h"
#include <QDebug>

QString ErrorController::CONTROLLER_NAME = QStringLiteral("ErrorController");

ErrorController::ErrorController(QObject *parent) : ControllerBase(parent)
{
}

QString ErrorController::getName() {
    return CONTROLLER_NAME;
}

QStringList ErrorController::getTopicPath() {
    return QStringList() << MQTT_PATH_ERRORS;
}

QStringList ErrorController::getLabelList() {
    CONVERT_LABEL_LIST(ERRORS_LABELS);
}

QVariant::Type ErrorController::getValueType(int index) {
    Q_UNUSED(index);

    return QVariant::StringList;
}

void ErrorController::onInit() {
}

void ErrorController::onValueChanged(int index, QVariant value) {
    qWarning() << Q_FUNC_INFO << index << value;
}
