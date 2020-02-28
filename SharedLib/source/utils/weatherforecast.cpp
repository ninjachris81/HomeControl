#include "include/utils/weatherforecast.h"
#include "include/utils/weatherforecastmanager.h"

WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent)
{

}

bool WeatherForecast::fromJson(QByteArray data) {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO << data;

    m_dates.clear();
    m_temps.clear();
    m_icons.clear();
    m_clouds.clear();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull()) {
        QJsonObject root = doc.object();

        QJsonArray list = root.value("list").toArray();

        for (int i=0;i<list.count();i++) {
            QJsonObject listEntry = list[i].toObject();

            m_dates.append(listEntry.value("dt").toInt());
            m_temps.append(listEntry.value("main").toObject().value("temp").toDouble());
            m_clouds.append(listEntry.value("clouds").toObject().value("all").toInt());

            QJsonValue weather = listEntry.value("weather");
            QString iconStr;
            if (weather.isArray()) {
                iconStr = weather[0].toObject().value("icon").toString();
            } else {
                iconStr = weather.toObject().value("icon").toString();
            }

            m_icons.append("http://openweathermap.org/img/wn/" + iconStr + ".png");
        }

        Q_EMIT(iconsChanged());
        Q_EMIT(tempsChanged());
        Q_EMIT(cloudsChanged());
        Q_EMIT(datesChanged());

        return true;
    } else {
        qCWarning(LG_WEATHERFORECAST) << "Failed to parse json" << data;
    }

    return false;
}

float WeatherForecast::getAverageTempOfTimespan(QDateTime from, QDateTime to) {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO << from << to;

    if (isValid()) {
        double returnVal = 0.0;
        int count = 0;

        for (int i=0;i<m_dates.count();i++) {
            if (m_dates[i]>=from.toSecsSinceEpoch() && m_dates[i]<=to.toSecsSinceEpoch()) {
                returnVal+=m_temps[i];
                count++;
            }
        }

        return static_cast<float>(returnVal / count);
    } else {
        qCWarning(LG_WEATHERFORECAST) << "Forecase invalid";
    }

    return -1;
}

float WeatherForecast::getAverageCloudsOfTimespan(QDateTime from, QDateTime to) {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO << from << to;

    if (isValid()) {
        double returnVal = 0.0;
        int count = 0;

        for (int i=0;i<m_clouds.count();i++) {
            if (m_dates[i]>=from.toSecsSinceEpoch() && m_dates[i]<=to.toSecsSinceEpoch()) {
                returnVal+=m_clouds[i];
                count++;
            }
        }

        return static_cast<float>(returnVal / count);
    } else {
        qCWarning(LG_WEATHERFORECAST) << "Forecase invalid";
    }

    return -1;
}

bool WeatherForecast::isValid() {
    qCDebug(LG_WEATHERFORECAST) << Q_FUNC_INFO << m_dates.count() << m_temps.count() << m_icons.count() << m_clouds.count();

    return (m_dates.count()==m_temps.count() && (m_dates.count()==m_icons.count()) && (m_dates.count()==m_clouds.count()) && (m_temps.count()>0));
    //return (m_temps.count()>0 && (m_dates.count()==m_temps.count()==m_icons.count()==m_clouds.count()));
}
