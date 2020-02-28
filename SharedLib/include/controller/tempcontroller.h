#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_TEMP_CONTROLLER)

class TempController : public ControllerBase
{
    Q_OBJECT
public:
    explicit TempController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return TEMP_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    qint64 getValueTrendLifetime(int index = -1);

    static QString CONTROLLER_NAME;

    QVariant value(int index);

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

signals:

public slots:

};

#endif // TEMPCONTROLLER_H
