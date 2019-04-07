#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include <QObject>
#include "controllermanager.h"
#include "constants_qt.h"

#include <QTimer>

class LogicController : public QObject
{
    Q_OBJECT
public:
    explicit LogicController(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    QTimer m_maintenanceTimer;

protected:
    ControllerManager* m_controllerManager;

    void startMaintenance(uint16_t interval);
    void stopMaintenance();

signals:

public slots:
    virtual void onMaintenance();

    virtual void onCommandReceived(MQTT_CMDS cmd);
};

#endif // LOGICCONTROLLER_H
