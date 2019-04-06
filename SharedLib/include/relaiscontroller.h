#ifndef RELAISCONTROLLER_H
#define RELAISCONTROLLER_H

#include "controllerbase.h"

class RelaisController : public ControllerBase
{
public:
    RelaisController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getValueType(int index = -1);

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // RELAISCONTROLLER_H
