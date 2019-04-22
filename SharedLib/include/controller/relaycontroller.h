#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include "controllerbase.h"

class RelayController : public ControllerBase
{
public:
    RelayController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getValueType(int index = -1);

    qint64 getValueLifetime(int index = -1);

    bool isValueOwner(int index = -1);

    static QString CONTROLLER_NAME;

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // RELAYCONTROLLER_H
