#ifndef EVENTCONTROLLER_H
#define EVENTCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_EVENT_CONTROLLER)

class EventController : public ControllerBase
{
    Q_OBJECT
public:
    explicit EventController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return EVENT_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

    bool isValueOwner(int index = -1);

    void raiseEvent(int index);

private:

signals:

public slots:
};

#endif // EVENTCONTROLLER_H
