#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include <QObject>
#include "controllermanager.h"
#include "../constants_qt.h"

#include <QTimer>

class LogicController : public QObject
{
    Q_OBJECT
public:
    explicit LogicController(ControllerManager *controllerManager, uint16_t interval = 0, QObject *parent = nullptr);

private:
    QTimer m_maintenanceTimer;

protected:
    uint16_t m_interval;
    ControllerManager* m_controllerManager;

    virtual void startMaintenance();
    virtual void stopMaintenance();

signals:

public slots:
    virtual void onMaintenance();

    virtual void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);

    virtual void onConnected();
};

#endif // LOGICCONTROLLER_H
