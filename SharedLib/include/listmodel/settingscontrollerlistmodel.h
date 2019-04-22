#ifndef SETTINGSCONTROLLERLISTMODEL_H
#define SETTINGSCONTROLLERLISTMODEL_H

#include "controllerlistmodel.h"
#include "../controller/controllerbase.h"

class SettingsControllerListModel : public ControllerListModel
{
public:
    SettingsControllerListModel(ControllerBase* controller);

};

Q_DECLARE_METATYPE( SettingsControllerListModel* );

#endif // SETTINGSCONTROLLERLISTMODEL_H
