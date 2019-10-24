#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include "controllerbase.h"

class SwitchController : public ControllerBase
{
public:
    SwitchController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getDefaultValueType();

    QString getEnumName();

    qint64 getValueLifetime(int index = -1);

    bool isValueOwner(int index = -1);

    static QString CONTROLLER_NAME;

    VALUE_BC_INTERVAL getValueBCInterval(int index=-1);

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // SWITCHCONTROLLER_H
