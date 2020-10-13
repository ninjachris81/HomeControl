#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include "controllerbase.h"
#include <QLoggingCategory>
#include "utils/rpigpiomanager.h"

Q_DECLARE_LOGGING_CATEGORY(LG_RELAY_CONTROLLER)

class RelayController : public ControllerBase
{
public:
    RelayController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return RELAY_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getDefaultValueType();

    QString getEnumName();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

private:
    RPIGpioManager gpioManager;

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // RELAYCONTROLLER_H
