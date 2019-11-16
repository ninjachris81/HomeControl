#ifndef WEATHERFORECASTMANAGER_H
#define WEATHERFORECASTMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QLoggingCategory>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "weatherforecast.h"

Q_DECLARE_LOGGING_CATEGORY(LG_WEATHERFORECAST)

class WeatherForecastManager : public QObject
{
    Q_OBJECT
public:
    explicit WeatherForecastManager(QString apiKey, QObject *parent = nullptr);

    Q_PROPERTY(WeatherForecast* forecast READ forecast NOTIFY forecastChanged)

    WeatherForecast* forecast() {
        return &m_currentForecast;
    }

    Q_INVOKABLE void requestForecast();

private:
    QNetworkAccessManager m_nam;

    QString m_apiKey;

    void executeRequest();

    WeatherForecast m_currentForecast;

private slots:
    void onFinished(QNetworkReply *reply);

signals:
    void forecastChanged();

public slots:
};

#endif // WEATHERFORECASTMANAGER_H
