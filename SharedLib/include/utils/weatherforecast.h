#ifndef WEATHERFORECAST_H
#define WEATHERFORECAST_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

class WeatherForecast : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QStringList icons READ icons NOTIFY iconsChanged)
    Q_PROPERTY(QList<int> dates READ dates NOTIFY datesChanged)
    Q_PROPERTY(QList<double> temps READ temps NOTIFY tempsChanged)
    Q_PROPERTY(QList<int> clouds READ clouds NOTIFY cloudsChanged)

    explicit WeatherForecast(QObject *parent = nullptr);

    bool fromJson(QByteArray data);

    QStringList icons() { return m_icons; }
    QList<int> dates() { return m_dates; }
    QList<double> temps() { return m_temps; }
    QList<int> clouds() { return m_clouds; }

    bool isValid();

    float getAverageTempOfTimespan(QDateTime from, QDateTime to);
    float getAverageCloudsOfTimespan(QDateTime from, QDateTime to);

private:
    QStringList m_icons;
    QList<int> m_dates;
    QList<double> m_temps;
    QList<int> m_clouds;

signals:
    void iconsChanged();
    void datesChanged();
    void tempsChanged();
    void cloudsChanged();

public slots:
};

#endif // WEATHERFORECAST_H
