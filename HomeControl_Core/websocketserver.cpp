#include "websocketserver.h"
#include <QWebSocketServer>
#include <QWebSocket>

Q_LOGGING_CATEGORY(LG_WS_SERVER, "WS Server");

WebsocketServer::WebsocketServer(AppConfiguration &appConfig, QObject *parent) : QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("HC Server"),
                                            QWebSocketServer::NonSecureMode, this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, static_cast<quint16>(appConfig.getInt(AppConfiguration::WS_PORT, 8080)))) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WebsocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebsocketServer::closed);
    } else {
        qCWarning(LG_WS_SERVER) << "Unable to start WS server" << m_pWebSocketServer->errorString();
    }

    connect(this, &WebsocketServer::broadcastValueChanged, this, &WebsocketServer::onBroadcastValueChanged);
}

WebsocketServer::~WebsocketServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WebsocketServer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    qCDebug(LG_WS_SERVER) << Q_FUNC_INFO << pSocket->peerAddress().toString();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebsocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebsocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebsocketServer::socketDisconnected);

    m_clients << pSocket;
}

void WebsocketServer::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        pClient->sendTextMessage(message);
    }
}

void WebsocketServer::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void WebsocketServer::socketDisconnected() {
    qCDebug(LG_WS_SERVER) << Q_FUNC_INFO;

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void WebsocketServer::onBroadcastValueChanged() {

    QString jsonStr = "";

    QListIterator<QWebSocket*> it(m_clients);
    while(it.hasNext()) {
        QWebSocket* ws = it.next();

        if (ws) {
            if (ws->isValid()) {
                ws->sendTextMessage(jsonStr);
            }
        }
    }
}
