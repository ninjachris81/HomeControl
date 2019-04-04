#ifndef HEATINGCONTROLLER_H
#define HEATINGCONTROLLER_H

#include <QObject>
#include "controllerbase.h"

class TempController : public ControllerBase
{
    Q_OBJECT
public:
    explicit TempController(AppConfiguration *appConfig, QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

signals:

public slots:

};

#endif // HEATINGCONTROLLER_H
