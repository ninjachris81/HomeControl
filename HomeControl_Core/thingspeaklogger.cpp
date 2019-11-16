#include "thingspeaklogger.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

Q_LOGGING_CATEGORY(LG_THINGSPEAK_LOGGERS, "ThingspeakLogger");

ThingSpeakLogger::ThingSpeakLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : LogicController(controllerManager, 0, parent)
{
    connect(&m_RequestTimer, &QTimer::timeout, this, &ThingSpeakLogger::checkNextRequest);
    m_RequestTimer.setInterval(20000);
    m_RequestTimer.start();

    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << m_nam.supportedSchemes();

    connect(&m_nam, &QNetworkAccessManager::finished, this, &ThingSpeakLogger::onFinished);

#ifndef QT_NO_SSL
    connect(&m_nam, &QNetworkAccessManager::sslErrors, this, &ThingSpeakLogger::onSslErrors);
#endif

    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));

    m_apiKeyTemp = appConfig->getString("THINGSPEAK_API_KEY_TEMP", "");
    m_apiKeyRelay = appConfig->getString("THINGSPEAK_API_KEY_RELAY", "");

    if (!m_apiKeyTemp.isEmpty()) {
        connect(m_tempController, &TempController::valueChanged, this, &ThingSpeakLogger::onTempValueChanged);
    } else {
        qCWarning(LG_THINGSPEAK_LOGGERS) << "Thingspeak (temp) disabled - no temp API key found";
    }

    if (!m_apiKeyRelay.isEmpty()) {
        connect(m_relayController, &RelayController::valueChanged, this, &ThingSpeakLogger::onRelayValueChanged);
    } else {
        qCWarning(LG_THINGSPEAK_LOGGERS) << "Thingspeak (relay) disabled - no relay API key found";
    }
}

void ThingSpeakLogger::executeRequest(QString query) {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << query;

    m_queue.enqueue(query);
}

void ThingSpeakLogger::_executeRequest(QString query) {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << query;

    QNetworkRequest req;

    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

    QUrl url;
    url.setScheme("http");
    url.setHost("api.thingspeak.com");
    url.setPath("/update");
    url.setQuery(query);

    qCDebug(LG_THINGSPEAK_LOGGERS) << "Requesting" << url;

    req.setUrl(url);

    m_nam.get(req);
}


QString ThingSpeakLogger::buildQuery(int index, QString value, QString apiKey) {
    return "api_key=" + apiKey + "&field" + QString::number(index + 1) + "=" + value;
}

void ThingSpeakLogger::onTempValueChanged(int index, QVariant value) {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << index << value;

    if (m_tempController->valueIsValid(index)) {
        executeRequest(buildQuery(index, value.toString(), m_apiKeyTemp));
    }
}

void ThingSpeakLogger::onRelayValueChanged(int index, QVariant value) {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << index << value;

    if (m_relayController->valueIsValid(index)) {
        executeRequest(buildQuery(index, value.toString(), m_apiKeyRelay));
    }
}

void ThingSpeakLogger::onMaintenance() {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO;
}

void ThingSpeakLogger::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    Q_UNUSED(cmd);
}

void ThingSpeakLogger::onFinished(QNetworkReply *reply) {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << reply->url().toString();

    if (reply->error()!=QNetworkReply::NoError) {
        qCWarning(LG_THINGSPEAK_LOGGERS) << "Failed to execute request" << reply->url().toString() << reply->errorString();
    } else {
        qCDebug(LG_THINGSPEAK_LOGGERS) << "Finished request" << reply->url().toString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    }

}

void ThingSpeakLogger::checkNextRequest() {
    qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << m_queue.size();

    if (!m_queue.isEmpty()) {
        _executeRequest(m_queue.dequeue());
    }
}

#ifndef QT_NO_SSL

    void ThingSpeakLogger::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
        qCDebug(LG_THINGSPEAK_LOGGERS) << Q_FUNC_INFO << reply->url().toString();

        QListIterator<QSslError> it(errors);

        while(it.hasNext()) {
            qCWarning(LG_THINGSPEAK_LOGGERS) << it.next().errorString();
        }
    }

#endif
