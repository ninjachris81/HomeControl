#ifndef SOLARLOGIC_H
#define SOLARLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/logiccontroller.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/brightnesscontroller.h"

#define SOLAR_LOGIC_INTERVAL 5000

Q_DECLARE_LOGGING_CATEGORY(LG_SOLAR_LOGIC)

class SolarLogic : public LogicController
{
    Q_OBJECT
public:
    explicit SolarLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;
    BrightnessController* m_brightnessController;

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);

};

#endif // SOLARLOGIC_H
