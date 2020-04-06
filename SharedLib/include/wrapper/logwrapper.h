#ifndef LOGWRAPPER_H
#define LOGWRAPPER_H

#include "controllerwrapper.h"
#include <QQmlEngine>
#include <QtGlobal>

#include "controllerwrapper.h"
#include "controller/logcontroller.h"

#include "sqllistmodel/logsqllistmodel.h"

class LogWrapper : public ControllerWrapper {
    Q_OBJECT

public:
    LogWrapper(LogController* controller);

    Q_PROPERTY(LogSqlListModel* logListModel READ logListModel NOTIFY logListModelChanged)

    LogSqlListModel *logListModel();

    Q_INVOKABLE void clearLog(int logFilter);

private:
    LogSqlListModel *m_logListModel;

signals:
    void logListModelChanged();

};

Q_DECLARE_METATYPE( LogWrapper* );


#endif // LOGWRAPPER_H
