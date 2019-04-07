#ifndef HEATINGLOGIC_H
#define HEATINGLOGIC_H

#include <QObject>
#include "controllermanager.h"
#include "logiccontroller.h"
#include "tempcontroller.h"
#include "relaiscontroller.h"
#include "settingscontroller.h"

class HeatingLogic : public LogicController
{
    Q_OBJECT
public:
    explicit HeatingLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelaisController* m_relaisController;

public slots:
    void onMaintenance();

    void onCommandReceived(MQTT_CMDS cmd);
};

#endif // HEATINGLOGIC_H
