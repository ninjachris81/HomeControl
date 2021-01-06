#ifndef DOORBELLLOGIC_H
#define DOORBELLLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/logiccontroller.h"
#include "controller/eventcontroller.h"

#define DOOR_BELL_LOGIC_INTERVAL 1000

Q_DECLARE_LOGGING_CATEGORY(LG_DOOR_BELL_LOGIC)

class DoorBellLogic : public LogicController
{
    Q_OBJECT
public:
    explicit DoorBellLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

    static QString DOOR_BELL_EVENTS_DB_NAME;

private:
    EventController *m_eventController;

private slots:
    void onEventValueChanged(int index, QVariant value);

signals:

public slots:
    void onMaintenance();

};

#endif // DOORBELLLOGIC_H
