#ifndef LOGCONTROLLERLISTMODEL_H
#define LOGCONTROLLERLISTMODEL_H

#include "controllerlistmodel.h"

class LogControllerListModel : public ControllerListModel
{
public:
    LogControllerListModel(ControllerBase* controller);

};

Q_DECLARE_METATYPE( LogControllerListModel* );

#endif // LOGCONTROLLERLISTMODEL_H
