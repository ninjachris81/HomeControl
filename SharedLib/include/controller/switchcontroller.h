#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

#include "utils/rpigpiomanager.h"

Q_DECLARE_LOGGING_CATEGORY(LG_SWITCH_CONTROLLER)

#define SWITCH_UPDATE_VALUE_INTERVAL 2000

class SwitchController : public ControllerBase
{
public:
    SwitchController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return SWITCH_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getDefaultValueType();

    QString getEnumName();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

    VALUE_BC_INTERVAL getValueBCInterval(int index=-1);

    bool isValueOwner(int index = -1);

private:
    RPIGpioManager gpioManager;

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

    void onScheduleUpdate();

};

#endif // SWITCHCONTROLLER_H
