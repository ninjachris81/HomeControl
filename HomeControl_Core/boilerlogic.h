#ifndef BOILERLOGIC_H
#define BOILERLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/infocontroller.h"

#include "utils/weatherforecastmanager.h"
#include "utils/appconfiguration.h"

#define BOILER_LOGIC_INTERVAL 10000

Q_DECLARE_LOGGING_CATEGORY(LG_BOILER_LOGIC)

class BoilerLogic : public LogicController
{
public:
    BoilerLogic(ControllerManager *controllerManager, AppConfiguration* appConfig, QObject *parent = nullptr);
    ~BoilerLogic();

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;
    InfoController* m_infoController;

    WeatherForecastManager* m_wfcManager;
    QTimer m_wfcTimer;

private slots:
    void onRefreshWFC();

    int getScheduledTempDelta();

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);

};

#endif // BOILERLOGIC_H
