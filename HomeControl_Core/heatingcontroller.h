#ifndef HEATINGCONTROLLER_H
#define HEATINGCONTROLLER_H

#include <QObject>
#include "controllerbase.h"

class HeatingController : public ControllerBase
{
    Q_OBJECT
public:
    explicit HeatingController(AppConfiguration *appConfig, QObject *parent = nullptr);

    QString getName();

    QMqttTopicFilter getTopicFilter();

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(QString topic, QVariant value);

signals:

public slots:

};

#endif // HEATINGCONTROLLER_H
