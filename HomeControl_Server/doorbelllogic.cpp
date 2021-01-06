#include "doorbelllogic.h"

#include <QSqlQuery>

#include "utils/databasemanager.h"

QString DoorBellLogic::DOOR_BELL_EVENTS_DB_NAME = "door_bell_events";

DoorBellLogic::DoorBellLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, DOOR_BELL_LOGIC_INTERVAL, parent)
{
    m_eventController = static_cast<EventController*>(controllerManager->getController(EventController::CONTROLLER_NAME));

    connect(m_eventController, &EventController::valueChanged, this, &DoorBellLogic::onEventValueChanged);

    QSqlQuery query(DatabaseManager::instance()->db());
    query.exec("CREATE TABLE IF NOT EXISTS " + DOOR_BELL_EVENTS_DB_NAME + " (ts INTEGER PRIMARY KEY, eventNum INTEGER)");
}

void DoorBellLogic::onMaintenance() {
}

void DoorBellLogic::onEventValueChanged(int index, QVariant value) {
    QSqlQuery query(DatabaseManager::instance()->db());
    if (index == EnumsDeclarations::EVENTS_DOOR_BELL) {
        query.prepare("INSERT INTO " + DOOR_BELL_EVENTS_DB_NAME + " (ts, eventNum) VALUES (:ts, :eventNum)");
        query.bindValue(":ts", QDateTime::currentSecsSinceEpoch());
        query.bindValue(":eventNum", value.toInt());
        query.exec();
    }
}
