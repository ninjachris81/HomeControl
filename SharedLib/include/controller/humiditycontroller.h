#ifndef HUMIDITYCONTROLLER_H
#define HUMIDITYCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_HUMIDITY_CONTROLLER)

class HumidityController : public ControllerBase
{
    Q_OBJECT
public:
    explicit HumidityController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return HUMIDITY_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

signals:

public slots:

};

#endif // HUMIDITYCONTROLLER_H
