#ifndef POWERLOGIC_H
#define POWERLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/logiccontroller.h"
#include "controller/powercontroller.h"

#define POWER_LOGIC_INTERVAL 10000

Q_DECLARE_LOGGING_CATEGORY(LG_POWER_LOGIC)

class PowerLogic : public LogicController
{
    Q_OBJECT
public:
    explicit PowerLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    PowerController* m_powerController;

private slots:
    void onCurrentValueChanged(int index, QVariant value);

signals:

public slots:
};

#endif // POWERLOGIC_H
