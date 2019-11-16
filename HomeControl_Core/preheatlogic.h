#ifndef WATERLOGIC_H
#define WATERLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/controllermanager.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/logiccontroller.h"
#include "controller/logcontroller.h"

#define PREHEAT_LOGIC_INTERVAL 1000

Q_DECLARE_LOGGING_CATEGORY(LG_PREHEAT_LOGIC)

class PreheatLogic : public LogicController
{
    Q_OBJECT
public:
    explicit PreheatLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

    void startPreheat(qint64 duration);

    void stopPreheat();

protected:
    void startMaintenance();

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;
    LogController* m_logController;

    qint64 m_lastStartRequest = 0;
    qint64 m_lastStartDuration = 0;

    bool isValidTankTemp();

signals:

private slots:

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // WATERLOGIC_H
