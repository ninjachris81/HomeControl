#ifndef POWERCONTROLLER_H
#define POWERCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_POWER_CONTROLLER)

class PowerController : public ControllerBase
{
    Q_OBJECT
public:
    explicit PowerController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return POWER_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

private:

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);


signals:

public slots:

};

#endif // POWERCONTROLLER_H
