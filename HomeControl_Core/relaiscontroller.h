#ifndef RELAISCONTROLLER_H
#define RELAISCONTROLLER_H

#include "controllerbase.h"

class RelaisController : public ControllerBase
{
public:
    RelaisController(AppConfiguration *appConfig, QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // RELAISCONTROLLER_H
