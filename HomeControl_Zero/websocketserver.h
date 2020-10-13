#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QLoggingCategory>

#include "utils/appconfiguration.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

Q_DECLARE_LOGGING_CATEGORY(LG_WS_SERVER)



/*
 * CURRENTLY NOT IN USE !!!!!
 *
*/


class WebsocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketServer(AppConfiguration &appConfig, QObject *parent = nullptr);
    ~WebsocketServer();

Q_SIGNALS:
    void closed();
    void broadcastValueChanged();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

    void onBroadcastValueChanged();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;

public slots:
};

#endif // WEBSOCKETSERVER_H
