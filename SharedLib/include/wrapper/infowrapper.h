#ifndef INFOWRAPPER_H
#define INFOWRAPPER_H

#include "controllerwrapper.h"
#include <QObject>
#include <QQmlEngine>
#include <QtGlobal>
#include <QDateTime>
#include "controller/infocontroller.h"

class InfoWrapper : public ControllerWrapper
{
    Q_OBJECT
public:
    explicit InfoWrapper(InfoController *controller);

    Q_PROPERTY(bool timeIsOffset READ timeIsOffset NOTIFY timeIsOffsetChanged)

    Q_PROPERTY(QDateTime systemTime READ systemTime NOTIFY systemTimeChanged)

    Q_PROPERTY(double systemTemp READ systemTemp NOTIFY systemTempChanged)

    Q_PROPERTY(double boilerTargetTemp READ boilerTargetTemp NOTIFY boilerTargetTempChanged)

    bool timeIsOffset();
    QDateTime systemTime();
    double systemTemp();
    double boilerTargetTemp();

signals:
    void timeIsOffsetChanged();
    void systemTimeChanged();
    void systemTempChanged();
    void boilerTargetTempChanged();

protected slots:
    void onTimeIsOffsetChanged(bool isOffset);

    void onValueChanged(int index, QVariant value);

};

Q_DECLARE_METATYPE( InfoWrapper* );

#endif // INFOWRAPPER_H
