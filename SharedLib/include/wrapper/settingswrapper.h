#ifndef SETTINGSWRAPPER_H
#define SETTINGSWRAPPER_H

#include "controllerwrapper.h"
#include <QObject>
#include <QQmlEngine>
#include <QtGlobal>
#include "constants_qt.h"

#include "controller/settingscontroller.h"

class SettingsWrapper : public ControllerWrapper {
    Q_OBJECT

public:
    SettingsWrapper(SettingsController* controller);
};

Q_DECLARE_METATYPE( SettingsWrapper* );


#endif // SETTINGSWRAPPER_H
