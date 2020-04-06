#include "include/utils/weatherforecastmanager.h"

Q_LOGGING_CATEGORY(LG_WEATHERFORECAST, "WeatherforeCast");

WeatherForecastManager* WeatherForecastManager::m_instance = nullptr;

WeatherForecastManager::WeatherForecastManager(QString apiKey, QObject *parent) : QObject(parent), m_apiKey(apiKey)
{
    WeatherForecastManager();
}

WeatherForecastManager::WeatherForecastManager(QObject *parent) {
    connect(&m_timer, &QTimer::timeout, this, &WeatherForecastManager::onAutoRefresh);
    connect(&m_nam, &QNetworkAccessManager::finished, this, &WeatherForecastManager::onFinished);
}

void WeatherForecastManager::requestForecast() {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO;

    executeRequest();
}

void WeatherForecastManager::setApiKey(QString apiKey) {
    m_apiKey = apiKey;
}

void WeatherForecastManager::enableAutoRefresh(int intervalMs) {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO << intervalMs;
    m_timer.setInterval(intervalMs);
    m_timer.start();
}

void WeatherForecastManager::disableAutoRefresh() {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO;
    m_timer.stop();
}

void WeatherForecastManager::onAutoRefresh() {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO;
    requestForecast();
}


void WeatherForecastManager::executeRequest() {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO;

    if (!m_apiKey.isEmpty()) {
        QNetworkRequest req;

        QUrl url;
        url.setScheme("http");
        url.setHost("api.openweathermap.org");
        url.setPath("/data/2.5/forecast");
        url.setQuery("id=2825297&units=metric&appid=" + m_apiKey);

        qCDebug(LG_WEATHERFORECAST) << "Requesting" << url;

        req.setUrl(url);

        m_nam.get(req);
    } else {
        qCWarning(LG_WEATHERFORECAST) << "No API key defined";
    }
}

void WeatherForecastManager::onFinished(QNetworkReply *reply) {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO << reply->url().toString();

    if (reply->error()!=QNetworkReply::NoError) {
        qCWarning(LG_WEATHERFORECAST) << "Failed to execute request" << reply->url().toString() << reply->errorString();
    } else {
        QByteArray content = reply->readAll();

        qCDebug(LG_WEATHERFORECAST) << "Finished request" << reply->url().toString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << content.length();

        if (m_currentForecast.fromJson(content)) {
            qCDebug(LG_WEATHERFORECAST) << "Loaded data" << m_currentForecast.isValid();
        } else {
            qCWarning(LG_WEATHERFORECAST) << "Failed to load data";
        }
        Q_EMIT(forecastChanged());
    }
}
