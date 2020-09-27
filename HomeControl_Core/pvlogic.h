#ifndef PVLOGIC_H
#define PVLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/logiccontroller.h"

#define PV_LOGIC_INTERVAL 10000

Q_DECLARE_LOGGING_CATEGORY(LG_PV_LOGIC)

class PvLogic : public LogicController
{
    Q_OBJECT
public:
    explicit PvLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

protected:
    void startMaintenance();

public slots:
    virtual void onMaintenance();

signals:

public slots:
};

#endif // PVLOGIC_H
