#include "thingspeaklogger.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

ThingSpeakLogger::ThingSpeakLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : LogicController(controllerManager, 0, parent)
{
    qDebug() << Q_FUNC_INFO;

    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));

    m_apiKeyTemp = appConfig->getString("THINGSPEAK_API_KEY_TEMP", "");
    m_apiKeyRelay = appConfig->getString("THINGSPEAK_API_KEY_RELAY", "");

    if (!m_apiKeyTemp.isEmpty()) {
        connect(m_tempController, &TempController::valueChanged, this, &ThingSpeakLogger::onTempValueChanged);
    } else {
        qWarning() << "Thingspeak disabled - no temp API key found";
    }

    if (!m_apiKeyRelay.isEmpty()) {
        connect(m_relayController, &RelayController::valueChanged, this, &ThingSpeakLogger::onRelayValueChanged);
    } else {
        qWarning() << "Thingspeak disabled - no relay API key found";
    }
}

void ThingSpeakLogger::executeRequest(QString query, QString apiKey) {
    qDebug() << Q_FUNC_INFO << query;

    QNetworkRequest req;

    QUrl url;
    url.setScheme("https");
    url.setHost("api.thingspeak.com");
    url.setPath("/update");
    url.setQuery("api_key=" + apiKey + "&" + query);

    qDebug() << "Requesting" << url;

    req.setUrl(url);

    QNetworkReply *resp = m_nam.get(req);
    if (resp->error()!=QNetworkReply::NoError) {
        qWarning() << "Failed to execute request" << resp->errorString();
    }
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


