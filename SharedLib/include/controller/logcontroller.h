#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QTcpServer>

#include "controllerbase.h"

#define LOG_PORT 8888

class LogController : public ControllerBase
{
public:
    LogController(QObject *parent = nullptr);

    enum LOGS_MODE {
        LOGS_SERVER,
        LOGS_CLIENT
    };

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getValueType(int index = -1);

    qint64 getValueLifetime(int index = -1);

    bool isValueOwner(int index = -1);

    void broadcastValues();

    void retrieveLog(QString host);

    static QString CONTROLLER_NAME;

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

    bool insertRecord(QDateTime date, int type, QString msg);

    bool checkTables();

protected slots:
    void onNewConnection();

    void onSettingsValueChanged(int index, QVariant value);

};

#endif // LOGCONTROLLER_H
