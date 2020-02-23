#ifndef BOILERLOGIC_H
#define BOILERLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"

#define BOILER_LOGIC_INTERVAL 5000

Q_DECLARE_LOGGING_CATEGORY(LG_BOILER_LOGIC)

class BoilerLogic : public LogicController
{
public:
    BoilerLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);

};

#endif // BOILERLOGIC_H
