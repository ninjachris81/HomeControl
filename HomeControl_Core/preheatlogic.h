#ifndef WATERLOGIC_H
#define WATERLOGIC_H

#include <QObject>
#include "controllermanager.h"
#include "tempcontroller.h"
#include "relaiscontroller.h"
#include "settingscontroller.h"
#include "logiccontroller.h"

class PreheatLogic : public LogicController
{
    Q_OBJECT
public:
    explicit PreheatLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    SettingsController* m_settingsController;
    TempController* m_tempController;
    RelaisController* m_relaisController;

signals:

private slots:

public slots:
    void onMaintenance();

    void onCommandReceived(MQTT_CMDS cmd);
};

#endif // WATERLOGIC_H
