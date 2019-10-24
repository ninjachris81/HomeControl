#ifndef INFOCONTROLLER_H
#define INFOCONTROLLER_H

#include <QObject>
#include <QTimer>

#include "controllerbase.h"

class InfoController : public ControllerBase
{
    Q_OBJECT
public:
    explicit InfoController(QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

    bool timeIsOffset();

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

    QTimer m_systemTimeTimer;

    bool m_timeIsOffset = false;

signals:

    void timeIsOffsetChanged(bool isOffset);

public slots:

};

#endif // INFOCONTROLLER_H
