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

    Q_PROPERTY(QDateTime serverTime READ serverTime NOTIFY serverTimeChanged)

    bool timeIsOffset();

    QDateTime serverTime();

signals:
    void timeIsOffsetChanged();

    void serverTimeChanged();

protected slots:
    void onTimeIsOffsetChanged(bool isOffset);

    void onValueChanged(int index, QVariant value);

};

Q_DECLARE_METATYPE( InfoWrapper* );

#endif // INFOWRAPPER_H
