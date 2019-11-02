#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <QObject>
#include "controllerbase.h"

class TempController : public ControllerBase
{
    Q_OBJECT
public:
    explicit TempController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    bool isValueOwner(int index = -1);

    static QString CONTROLLER_NAME;

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

signals:

public slots:

};

#endif // TEMPCONTROLLER_H
