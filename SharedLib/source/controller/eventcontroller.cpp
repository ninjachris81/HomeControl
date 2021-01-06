#include "controller/eventcontroller.h"
#include "controller/controllermanager.h"

QString EventController::CONTROLLER_NAME = QStringLiteral("EventController");
Q_LOGGING_CATEGORY(LG_EVENT_CONTROLLER, "EventController");

EventController::EventController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
{
}

QString EventController::getName() {
    return CONTROLLER_NAME;
}

QStringList EventController::getTopicPath() {
    return QStringList() << MQTT_PATH_EVENTS;
}

QStringList EventController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::EVENTS_LABELS);
}

QString EventController::getEnumName() {
    return "MQTT_EVENTS";
}

QVariant::Type EventController::getDefaultValueType() {
    return QVariant::Int;
}

qint64 EventController::getValueLifetime(int index) {
    switch (index) {
    case EnumsDeclarations::BRIGHTNESSES_SOLAR:
        return LIFETIME_MID;
    default:
        return LIFETIME_UNLIMITED;
    }
}

bool EventController::isValueOwner(int index) {
    switch (index) {
    case EnumsDeclarations::BRIGHTNESSES_OUTSIDE:
        return m_parent->deviceId()==DEV_ID_ZERO;
    default:
        return ControllerBase::isValueOwner(index);
    }
}

void EventController::raiseEvent(int index) {
    qDebug() << Q_FUNC_INFO << index;

    if (checkIndex(index)) {
        int currentValue = value(index).toInt() + 1;

        qDebug() << "Event" << currentValue;

        setValue(index, currentValue);
        publishValue(index);
    }
}
