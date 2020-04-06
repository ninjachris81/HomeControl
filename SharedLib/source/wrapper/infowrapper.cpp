#include "include/wrapper/infowrapper.h"
#include "include/controller/controllermanager.h"
#include "include/utils/forecastutils.h"

InfoWrapper::InfoWrapper(InfoController *controller, SettingsController *settingsController) : ControllerWrapper(controller), m_settingsController(settingsController)
{
    connect(static_cast<InfoController*>(controller), &InfoController::timeIsOffsetChanged, this, &InfoWrapper::onTimeIsOffsetChanged);
    connect(static_cast<InfoController*>(controller), &InfoController::valueChanged, this, &InfoWrapper::onValueChanged);

    connect(m_settingsController, &SettingsController::valueChanged, this, &InfoWrapper::onSettingsValueChanged);
}

bool InfoWrapper::timeIsOffset() {
    return static_cast<InfoController*>(m_controller)->timeIsOffset();
}

void InfoWrapper::onTimeIsOffsetChanged(bool isOffset) {
    Q_UNUSED(isOffset);
    Q_EMIT(timeIsOffsetChanged());
}

void InfoWrapper::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    Q_UNUSED(value);

    switch (index) {
    case EnumsDeclarations::INFOS_SYSTEM_TIME:
        Q_EMIT(systemTimeChanged());
        break;
    case EnumsDeclarations::INFOS_SYSTEM_TEMP:
        Q_EMIT(systemTempChanged());
        break;
    case EnumsDeclarations::INFOS_BOILER_TARGET_TEMP:
        Q_EMIT(boilerTargetTempChanged());
        break;
    case EnumsDeclarations::INFOS_SUN_EXPECTED:
        Q_EMIT(sunExpectedChanged());
        break;
    case EnumsDeclarations::INFOS_AVG_TEMP_FORECAST:
        Q_EMIT(avgTempForecastChanged());
        checkTankTempScheduleReady();
        break;
    case EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST:
        Q_EMIT(avgCloudsForecastChanged());
        checkTankTempScheduleReady();
        break;
    default:
        break;
    }
}

void InfoWrapper::onSettingsValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    switch(index) {
    case EnumsDeclarations::SETTINGS_TANK_MIN_TEMP:
    case EnumsDeclarations::SETTINGS_BOILER_SCHEDULE:
    case EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT:
    case EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD:
    case EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD:
        checkTankTempScheduleReady();
        break;
    default:
        break;
    }
}


QDateTime InfoWrapper::systemTime() {
    return QDateTime::fromString(m_controller->value(EnumsDeclarations::INFOS_SYSTEM_TIME).toString(), Qt::ISODate);
}

double InfoWrapper::systemTemp() {
    return m_controller->value(EnumsDeclarations::INFOS_SYSTEM_TEMP).toDouble();
}

double InfoWrapper::boilerTargetTemp() {
    return m_controller->value(EnumsDeclarations::INFOS_BOILER_TARGET_TEMP).toDouble();
}

int InfoWrapper::sunExpected() {
    return m_controller->value(EnumsDeclarations::INFOS_SUN_EXPECTED).toInt();
}

int InfoWrapper::avgTempForecast() {
    return m_controller->value(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST).toInt();
}

int InfoWrapper::avgCloudsForecast() {
    return m_controller->value(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST).toInt();
}

QVariantList InfoWrapper::tankTempSchedule() {
    QVariantList returnList;

    int minTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP).toInt();
    QString bs = m_settingsController->value(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE).toString();
    int sunExpSubstract = m_settingsController->value(EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT).toInt();
    float avgTempTomorrow = m_controller->value(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST).toFloat();
    float avgCloudsTomorrow = m_controller->value(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST).toFloat();

    bool sunExpected = ForecastUtils::calculateSunExpected(avgTempTomorrow, m_settingsController->value(EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD).toInt(), avgCloudsTomorrow, m_settingsController->value(EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD).toInt());

    for (int i=0;i<24;i++) {
        returnList.append(ForecastUtils::calculateTargetTemp(i, bs, minTemp, sunExpected, sunExpSubstract));
    }

    return returnList;
}

void InfoWrapper::checkTankTempScheduleReady() {
    if (!m_settingsController->value(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP).isValid() || m_settingsController->value(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP).toInt()==0) return;
    if (!m_settingsController->value(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE).isValid() || m_settingsController->value(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE).toString().isEmpty()) return;
    if (!m_settingsController->value(EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT).isValid()) return;
    if (!m_settingsController->value(EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD).isValid()) return;
    if (!m_settingsController->value(EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD).isValid()) return;
    if (!m_controller->value(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST).isValid()) return;
    if (!m_controller->value(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST).isValid()) return;


    qDebug() << m_settingsController->value(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP).isValid();
    qDebug() << m_settingsController->value(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE).isValid();
    qDebug() << m_settingsController->value(EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT).isValid();
    qDebug() << m_settingsController->value(EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD).isValid();
    qDebug() << m_settingsController->value(EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD).isValid();
    qDebug() << m_controller->value(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST).isValid();
    qDebug() << m_controller->value(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST).isValid();

    qDebug() << "Tank temp schedule ready";

    Q_EMIT(tankTempScheduleReady());
    Q_EMIT(tankTempScheduleChanged());
}
