#include "include/utils/forecastutils.h"
#include <QDebug>

ForecastUtils::ForecastUtils()
{

}

double ForecastUtils::calculateTargetTemp(int currentHour, QString bs, int minTemp, bool sunExpected, int sunExpSubstract) {
    int scheduledTempDelta = getScheduledTempDelta(bs, currentHour);
    return minTemp + scheduledTempDelta + (sunExpected ? -sunExpSubstract : 0);
}

int ForecastUtils::getScheduledTempDelta(QString bs, int hour) {
    if (!bs.isEmpty()) {
        QStringList hours = bs.split(" ", QString::SkipEmptyParts);
        if (hours.count()==24) {
            return hours.at(hour).toInt();
        }
    }

    return 0;
}

bool ForecastUtils::calculateSunExpected(float avgTempTomorrow, int tempExpThreshold, float avgCloudsTomorrow, int cloudsExpThreshold) {
    return avgTempTomorrow>=tempExpThreshold && avgCloudsTomorrow<=cloudsExpThreshold;
}
