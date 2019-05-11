#include "thingspeaklogger.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

ThingSpeakLogger::ThingSpeakLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : LogicController(controllerManager, 0, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));

    m_apiKey = appConfig->getString("THINGSPEAK_API_KEY", "");

    if (!m_apiKey.isEmpty()) {
        connect(m_tempController, &TempController::valueChanged, this, &ThingSpeakLogger::onTempValueChanged);
    } else {
        qWarning() << "Thingspeak disabled - no API key found";
    }
}

void ThingSpeakLogger::onTempValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (m_tempController->valueIsValid(index)) {
        QNetworkRequest req;

        QUrl url;

        url.setScheme("https");
        url.setHost("api.thingspeak.com");
        url.setPath("/update");
        url.setQuery("api_key=" + m_apiKey + "&field" + QString::number(index + 1) + "=" + value.toString());

        qDebug() << "Requesting" << url;

        req.setUrl(url);

        QNetworkReply *resp = m_nam.get(req);
        if (resp->error()!=QNetworkReply::NoError) {
            qWarning() << "Failed to execute request" << resp->errorString();
        }
    }
}

void ThingSpeakLogger::onMaintenance() {

}

void ThingSpeakLogger::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    Q_UNUSED(cmd);
}
