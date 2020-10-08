#ifndef CURRENTCONTROLLER_H
#define CURRENTCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_CURRENT_CONTROLLER)

class CurrentController : public ControllerBase
{
    Q_OBJECT
public:
    explicit CurrentController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return CURRENT_CONTROLLER;
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

#endif // CURRENTCONTROLLER_H
