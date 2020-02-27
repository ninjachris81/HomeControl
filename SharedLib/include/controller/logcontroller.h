#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QTcpServer>

#include "controllerbase.h"

#include "utils/qsqldatabasewrapper.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_LOG_CONTROLLER)

#define LOG_PORT 8888

class LogController : public ControllerBase
{
    Q_OBJECT

public:
    LogController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return LOG_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    QString getEnumName();

    void broadcastValues();

    void refreshLog();

    void addLog(EnumsDeclarations::MQTT_LOGS type, QString source, QString msg = "");

    static QString CONTROLLER_NAME;

    static QString DB_CONN_LOGS;

    static QString DB_TABLE_LOGS;

protected:
    QSqlDatabase m_db;
    QTcpServer *m_tcpServer;

    bool hasSetSupport() {
        return true;
    }

    void onInit();

    void onValueChanged(int index, QVariant value);

    /*virtual*/ void onSetReceived(int index, QVariant value);

    bool insertRecord(QDateTime date, int type, QString source, QString msg);

    bool checkTables();

    void retrieveLog();

    void onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd);

    void clearLog(int typeFilter = -1);

protected slots:
    void onNewConnection();

    void onSettingsValueChanged(int index, QVariant value);

signals:
    void logDataChanged();

};

#endif // LOGCONTROLLER_H
