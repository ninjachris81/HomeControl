#ifndef WEATHERFORECAST_H
#define WEATHERFORECAST_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class WeatherForecast : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QStringList icons READ icons NOTIFY iconsChanged)
    Q_PROPERTY(QList<int> dates READ dates NOTIFY datesChanged)
    Q_PROPERTY(QList<double> temps READ temps NOTIFY tempsChanged)

    explicit WeatherForecast(QObject *parent = nullptr);

    bool fromJson(QByteArray data);

    QStringList icons() { return m_icons; }
    QList<int> dates() { return m_dates; }
    QList<double> temps() { return m_temps; }

private:
    QStringList m_icons;
    QList<int> m_dates;
    QList<double> m_temps;

signals:
    void iconsChanged();
    void datesChanged();
    void tempsChanged();

public slots:
};

#endif // WEATHERFORECAST_H
