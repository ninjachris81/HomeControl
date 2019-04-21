#ifndef ERRORCONTROLLER_H
#define ERRORCONTROLLER_H

#include <QObject>
#include "controllerbase.h"

class ErrorController : public ControllerBase
{
public:
    ErrorController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getValueType(int index = -1);

    static QString CONTROLLER_NAME;

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // ERRORCONTROLLER_H
