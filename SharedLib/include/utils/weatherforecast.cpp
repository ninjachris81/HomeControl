#include "weatherforecast.h"
#include "weatherforecastmanager.h"

WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent)
{

}

bool WeatherForecast::fromJson(QByteArray data) {
    qCDebug(LG_WEATHERFORECAST()) << Q_FUNC_INFO << data;

    m_dates.clear();
    m_temps.clear();
    m_icons.clear();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull()) {
        QJsonObject root = doc.object();

        QJsonArray list = root.value("list").toArray();

        for (int i=0;i<list.count();i++) {
            QJsonObject listEntry = list[i].toObject();

            int dt = listEntry.value("dt").toInt();

            m_dates.append(dt);

            m_temps.append(listEntry.value("main").toObject().value("temp").toDouble());

            QJsonValue weather = listEntry.value("weather");
            QString iconStr;
            if (weather.isArray()) {
                iconStr = weather[0].toObject().value("icon").toString();
            } else {
                iconStr = weather.toObject().value("icon").toString();
            }

            m_icons.append("http://openweathermap.org/img/wn/" + iconStr + ".png");
        }

        qCDebug(LG_WEATHERFORECAST) << m_icons;

        Q_EMIT(iconsChanged());

        return true;
    } else {
        qCWarning(LG_WEATHERFORECAST) << "Failed to parse json" << data;
    }

    return false;
}
