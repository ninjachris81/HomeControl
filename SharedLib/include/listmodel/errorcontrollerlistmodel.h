#ifndef ERRORCONTROLLERLISTMODEL_H
#define ERRORCONTROLLERLISTMODEL_H

#include "controllerlistmodel.h"

class ErrorControllerListModel : public ControllerListModel
{
public:
    ErrorControllerListModel(ControllerBase* controller);

};

Q_DECLARE_METATYPE( ErrorControllerListModel* );

#endif // ERRORCONTROLLERLISTMODEL_H
