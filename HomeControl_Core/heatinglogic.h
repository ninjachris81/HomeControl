#ifndef HEATINGLOGIC_H
#define HEATINGLOGIC_H

#include <QObject>
#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"

//#define HEATING_TOGGLE_ON_DURATION 60000
#define HEATING_LOGIC_INTERVAL 5000

class HeatingLogic : public LogicController
{
    Q_OBJECT
public:
    explicit HeatingLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;

    qint64 m_lastHeatOff = 0;

protected:
    void startMaintenance();

    bool isValidMonth();

    bool isValidTime();

    bool isValidTankTemp();

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // HEATINGLOGIC_H
