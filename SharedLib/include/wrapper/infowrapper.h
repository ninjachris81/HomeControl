#ifndef INFOWRAPPER_H
#define INFOWRAPPER_H

#include "controllerwrapper.h"
#include <QObject>
#include <QQmlEngine>
#include <QtGlobal>
#include <QDateTime>
#include "controller/infocontroller.h"
#include "controller/settingscontroller.h"

class InfoWrapper : public ControllerWrapper
{
    Q_OBJECT
public:
    explicit InfoWrapper(InfoController *controller, SettingsController *settingsController);

    Q_PROPERTY(bool timeIsOffset READ timeIsOffset NOTIFY timeIsOffsetChanged)
    Q_PROPERTY(QDateTime systemTime READ systemTime NOTIFY systemTimeChanged)
    Q_PROPERTY(double systemTemp READ systemTemp NOTIFY systemTempChanged)
    Q_PROPERTY(double boilerTargetTemp READ boilerTargetTemp NOTIFY boilerTargetTempChanged)
    Q_PROPERTY(int sunExpected READ sunExpected NOTIFY sunExpectedChanged)
    Q_PROPERTY(int avgTempForecast READ avgTempForecast NOTIFY avgTempForecastChanged)
    Q_PROPERTY(int avgCloudsForecast READ avgCloudsForecast NOTIFY avgCloudsForecastChanged)
    Q_PROPERTY(QVariantList tankTempSchedule READ tankTempSchedule NOTIFY tankTempScheduleChanged)

    bool timeIsOffset();
    QDateTime systemTime();
    double systemTemp();
    double boilerTargetTemp();
    int sunExpected();
    int avgTempForecast();
    int avgCloudsForecast();
    QVariantList tankTempSchedule();

signals:
    void timeIsOffsetChanged();
    void systemTimeChanged();
    void systemTempChanged();
    void boilerTargetTempChanged();
    void sunExpectedChanged();
    void avgTempForecastChanged();
    void avgCloudsForecastChanged();
    void tankTempScheduleChanged();
    void tankTempScheduleReady();

protected slots:
    void onTimeIsOffsetChanged(bool isOffset);

    void onValueChanged(int index, QVariant value);
    void onSettingsValueChanged(int index, QVariant value);

private:
    SettingsController *m_settingsController;

    void checkTankTempScheduleReady();

};

Q_DECLARE_METATYPE( InfoWrapper* );

#endif // INFOWRAPPER_H
