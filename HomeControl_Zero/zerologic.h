#ifndef ZEROLOGIC_H
#define ZEROLOGIC_H

#include <QObject>
#include <QLoggingCategory>
#include "controller/logiccontroller.h"

#define ZERO_LOGIC_INTERVAL 5000

Q_DECLARE_LOGGING_CATEGORY(LG_ZEROLOGIC_LOGIC)


class ZeroLogic : public LogicController
{
    Q_OBJECT
public:
    explicit ZeroLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

signals:


public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // ZEROLOGIC_H
