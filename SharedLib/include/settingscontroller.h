#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "controllerbase.h"

class SettingsController : public ControllerBase
{
public:
    SettingsController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getValueType(int index);

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);
};

#endif // SETTINGSCONTROLLER_H
