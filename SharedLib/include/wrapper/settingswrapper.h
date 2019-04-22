#ifndef SETTINGSWRAPPER_H
#define SETTINGSWRAPPER_H

#include "controllerwrapper.h"
#include <QQmlEngine>
#include <QtGlobal>
#include "../constants_qt.h"

class SettingsWrapper : public ControllerWrapper {
public:
    SettingsWrapper(ControllerBase* controller);
};

Q_DECLARE_METATYPE( SettingsWrapper* );


#endif // SETTINGSWRAPPER_H
