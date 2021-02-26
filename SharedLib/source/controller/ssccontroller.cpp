#include "include/controller/ssccontroller.h"
#include "include/controller/controllermanager.h"
#include <QMetaEnum>

QString SSCController::CONTROLLER_NAME = QStringLiteral("SSCController");
Q_LOGGING_CATEGORY(LG_SSC_CONTROLLER, "SSCController");

SSCController::SSCController(QObject *parent) : ControllerBase (ControllerBase::VALUE_OWNER_SERVER, parent)
{
}

QString SSCController::getName() {
    return CONTROLLER_NAME;
}

QStringList SSCController::getTopicPath() {
    return QStringList() << MQTT_PATH_SSC;
}

QStringList SSCController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::SSC_VARIABLES_LABELS);
}

QVariant::Type SSCController::getDefaultValueType() {
    return QVariant::Int;
}

QString SSCController::getEnumName() {
    return "MQTT_SSC_VARIABLES";
}

qint64 SSCController::getValueLifetime(int index) {
    Q_UNUSED(index);
    return LIFETIME_MID;
}

ControllerBase::VALUE_BC_INTERVAL SSCController::getValueBCInterval(int index) {
    Q_UNUSED(index);
    return ControllerBase::VALUE_BC_SLOW;
}

void SSCController::onInit() {
}

bool SSCController::isValueOwner(int index) {
    switch (index) {
    default:
        return ControllerBase::isValueOwner(index);
    }
}

void SSCController::onValueChanged(int index, QVariant value) {
    qCDebug(LG_SSC_CONTROLLER) << Q_FUNC_INFO << index << value;

    switch(index) {
    }
}
