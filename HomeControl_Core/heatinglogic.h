#ifndef HEATINGLOGIC_H
#define HEATINGLOGIC_H

#include <QObject>
#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"

class HeatingLogic : public LogicController
{
    Q_OBJECT
public:
    explicit HeatingLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // HEATINGLOGIC_H
