#ifndef WATERLOGIC_H
#define WATERLOGIC_H

#include <QObject>

#include "controller/controllermanager.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/logiccontroller.h"

class PreheatLogic : public LogicController
{
    Q_OBJECT
public:
    explicit PreheatLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

    void startPreheat(qint64 duration);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelayController* m_relayController;

    qint64 m_lastStartRequest = 0;
    qint64 m_lastStartDuration = 0;

signals:

private slots:

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // WATERLOGIC_H
