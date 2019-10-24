#ifndef INFOWRAPPER_H
#define INFOWRAPPER_H

#include "controllerwrapper.h"
#include <QObject>
#include <QQmlEngine>
#include <QtGlobal>
#include "../controller/infocontroller.h"

class InfoWrapper : public ControllerWrapper
{
    Q_OBJECT
public:
    explicit InfoWrapper(InfoController *controller);

    Q_PROPERTY(bool timeIsOffset READ timeIsOffset NOTIFY timeIsOffsetChanged)

    bool timeIsOffset();

signals:
    void timeIsOffsetChanged();

protected slots:
    void onTimeIsOffsetChanged(bool isOffset);

};

Q_DECLARE_METATYPE( InfoWrapper* );

#endif // INFOWRAPPER_H
