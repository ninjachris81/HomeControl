#include "thingspeaklogger.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

ThingSpeakLogger::ThingSpeakLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : LogicController(controllerManager, 0, parent)
{
    m_nam = new QNetworkAccessManager(this);

    qDebug() << Q_FUNC_INFO << m_nam->supportedSchemes();

    connect(m_nam, &QNetworkAccessManager::finished, this, &ThingSpeakLogger::onFinished);

#ifndef QT_NO_SSL
    connect(m_nam, &QNetworkAccessManager::sslErrors, this, &ThingSpeakLogger::onSslErrors);
#endif

    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));

    m_apiKeyTemp = appConfig->getString("THINGSPEAK_API_KEY_TEMP", "");
    m_apiKeyRelay = appConfig->getString("THINGSPEAK_API_KEY_RELAY", "");

    if (!m_apiKeyTemp.isEmpty()) {
        connect(m_tempController, &TempController::valueChanged, this, &ThingSpeakLogger::onTempValueChanged);
    } else {
        qWarning() << "Thingspeak (temp) disabled - no temp API key found";
    }

    if (!m_apiKeyRelay.isEmpty()) {
        connect(m_relayController, &RelayController::valueChanged, this, &ThingSpeakLogger::onRelayValueChanged);
    } else {
        qWarning() << "Thingspeak (relay) disabled - no relay API key found";
    }
}

void ThingSpeakLogger::executeRequest(QString query, QString apiKey) {
    qDebug() << Q_FUNC_INFO << query;

    QNetworkRequest req;

    QUrl url;
    url.setScheme("http");
    url.setHost("api.thingspeak.com");
    url.setPath("/update");
    url.setQuery("api_key=" + apiKey + "&" + query);

    qDebug() << "Requesting" << url;

    req.setUrl(url);

    m_nam->get(req);
}

void ThingSpeakLogger::onTempValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (m_tempController->valueIsValid(index)) {
        executeRequest("field" + QString::number(index + 1) + "=" + value.toString(), m_apiKeyTemp);
    }
}

void ThingSpeakLogger::onRelayValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (m_relayController->valueIsValid(index)) {
        executeRequest("field" + QString::number(index + 1) + "=" + value.toString(), m_apiKeyRelay);
    }
}

void ThingSpeakLogger::onMaintenance() {
    qDebug() << Q_FUNC_INFO;
}

void ThingSpeakLogger::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    Q_UNUSED(cmd);
}

void ThingSpeakLogger::onFinished(QNetworkReply *reply) {
    qDebug() << Q_FUNC_INFO << reply->url().toString();

    if (reply->error()!=QNetworkReply::NoError) {
        qWarning() << "Failed to execute request" << reply->url().toString() << reply->errorString();
    } else {
        qDebug() << "Finished request" << reply->url().toString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    }
}

#ifndef QT_NO_SSL

    void ThingSpeakLogger::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
        qDebug() << Q_FUNC_INFO << reply->url().toString();

        QListIterator<QSslError> it(errors);

        while(it.hasNext()) {
            qWarning() << it.next().errorString();
        }
    }

#endif
