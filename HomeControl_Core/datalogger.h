#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QLoggingCategory>
#include <QTimer>
#include <QSqlDatabase>

#include "controller/controllerbase.h"

Q_DECLARE_LOGGING_CATEGORY(LG_DATALOGGER)

class DataLogger : public QObject
{
    Q_OBJECT
public:
    explicit DataLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent = nullptr);

    void registerValue(ControllerBase *controller, int valueIndex);

    static QString DB_TABLE_LOGS;

    static QString DB_CONN_LOGS;

private:
    QMultiMap<ControllerBase*, int> m_Values;
    ControllerManager *m_controllerManager;

    QTimer m_LogTimer;
    QTimer m_LimitTimer;

    QSqlDatabase m_db;

    quint16 m_entryLimit;

    void addEntry(ControllerBase::CONTROLLER_TYPE controllerType, int index, QVariant v);

private slots:
    void onLogData();

    void onCheckLimit();

signals:

public slots:
};

#endif // DATALOGGER_H
