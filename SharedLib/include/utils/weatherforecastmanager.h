#ifndef WEATHERFORECASTMANAGER_H
#define WEATHERFORECASTMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QLoggingCategory>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

#include "weatherforecast.h"

Q_DECLARE_LOGGING_CATEGORY(LG_WEATHERFORECAST)

class WeatherForecastManager : public QObject
{
    Q_OBJECT
public:
    explicit WeatherForecastManager(QString apiKey, QObject *parent = nullptr);
    WeatherForecastManager(QObject *parent = nullptr);

    Q_PROPERTY(WeatherForecast* forecast READ forecast NOTIFY forecastChanged)

    static WeatherForecastManager* m_instance;

    static WeatherForecastManager* instance() {
        if (m_instance==nullptr) {
            m_instance = new WeatherForecastManager();
        }

        return m_instance;
    }

    void setApiKey(QString apiKey);

    WeatherForecast* forecast() {
        return &m_currentForecast;
    }

    void enableAutoRefresh(int intervalMs);
    void disableAutoRefresh();

    Q_INVOKABLE void requestForecast();

private:
    QNetworkAccessManager m_nam;
    QTimer m_timer;

    QString m_apiKey;

    void executeRequest();

    WeatherForecast m_currentForecast;

private slots:
    void onFinished(QNetworkReply *reply);
    void onAutoRefresh();

signals:
    void forecastChanged();

public slots:
};

#endif // WEATHERFORECASTMANAGER_H
