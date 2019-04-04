#ifndef ERRORCONTROLLER_H
#define ERRORCONTROLLER_H

#include <QObject>
#include "controllerbase.h"

class ErrorController : public ControllerBase
{
public:
    ErrorController(AppConfiguration* appConfig, QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // ERRORCONTROLLER_H
