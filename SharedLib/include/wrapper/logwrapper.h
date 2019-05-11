#ifndef LOGWRAPPER_H
#define LOGWRAPPER_H

#include "controllerwrapper.h"
#include <QQmlEngine>
#include <QtGlobal>

#include "controllerwrapper.h"
#include "../controller/logcontroller.h"

class LogWrapper : public ControllerWrapper {
    Q_OBJECT

public:
    LogWrapper(LogController* controller);

    Q_INVOKABLE void refreshLog();

    Q_INVOKABLE void clearLog(int logFilter);

};

Q_DECLARE_METATYPE( LogWrapper* );


#endif // LOGWRAPPER_H
