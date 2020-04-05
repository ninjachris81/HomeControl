#ifndef FORECASTUTILS_H
#define FORECASTUTILS_H

#include <QObject>

class ForecastUtils
{
public:
    ForecastUtils();

    static double calculateTargetTemp(int currentHour, QString bs, int minTemp, bool sunExpected, int sunExpSubstract);

    static int getScheduledTempDelta(QString bs, int hour);

    static bool calculateSunExpected(float avgTempTomorrow, int tempExpThreshold, float avgCloudsTomorrow, int cloudsExpThreshold);

};

#endif // FORECASTUTILS_H
