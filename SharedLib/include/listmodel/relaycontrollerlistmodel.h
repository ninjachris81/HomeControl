#ifndef RELAYCONTROLLERLISTMODEL_H
#define RELAYCONTROLLERLISTMODEL_H

#include "controllerlistmodel.h"

class RelayControllerListModel : public ControllerListModel
{
public:
    RelayControllerListModel(ControllerBase* controller);

};

Q_DECLARE_METATYPE( RelayControllerListModel* );

#endif // RELAYCONTROLLERLISTMODEL_H
