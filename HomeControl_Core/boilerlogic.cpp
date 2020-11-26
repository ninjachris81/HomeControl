#include "boilerlogic.h"
#include <QDateTime>

Q_LOGGING_CATEGORY(LG_BOILER_LOGIC, "BoilerLogic");

BoilerLogic::BoilerLogic(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : LogicController(controllerManager, BOILER_LOGIC_INTERVAL, parent)
{
    Q_UNUSED(appConfig)
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));
    m_infoController = static_cast<InfoController*>(controllerManager->getController(InfoController::CONTROLLER_NAME));

    m_wfcManager = WeatherForecastManager::instance();
    m_wfcManager->enableAutoRefresh(30 * 60000);
    m_wfcManager->requestForecast();
}

void BoilerLogic::onMaintenance() {
    bool boilerOn = false;
    double targetTemp = 0;
    EnumsDeclarations::VALIDATED_BOOL sunExpectedVB = EnumsDeclarations::BOOL_INVALID;

    if (m_tempController->value(EnumsDeclarations::TEMPS_TANK).isValid()) {
        bool sunExpected = false;

        if (m_wfcManager->forecast()->isValid()) {
            // calculate wfc

            int daysAdd = QTime::currentTime().hour()>11 ? 1 : 0;
            QDate tomorrowOrToday = QDateTime::currentDateTime().date().addDays(daysAdd);

            QDateTime from = QDateTime(tomorrowOrToday, QTime(8, 0, 0));
            QDateTime to = QDateTime(tomorrowOrToday, QTime(17, 0, 0));

            float avgTempTomorrow = m_wfcManager->forecast()->getAverageTempOfTimespan(from, to);
            qCDebug(LG_BOILER_LOGIC) << "Temp tomorrow" << avgTempTomorrow;

            float avgCloudsTomorrow = m_wfcManager->forecast()->getAverageCloudsOfTimespan(from, to);
            qCDebug(LG_BOILER_LOGIC) << "Clouds tomorrow" << avgCloudsTomorrow;

            m_infoController->setValue(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST, static_cast<int>(avgTempTomorrow));
            m_infoController->setValue(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST, static_cast<int>(avgCloudsTomorrow));

            sunExpected = ForecastUtils::calculateSunExpected(avgTempTomorrow, m_settingsController->value(EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD).toInt(), avgCloudsTomorrow, m_settingsController->value(EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD).toInt());
            qCDebug(LG_BOILER_LOGIC) << "Sun expected" << sunExpected;

            sunExpectedVB = sunExpected ? EnumsDeclarations::BOOL_TRUE : EnumsDeclarations::BOOL_FALSE;
        } else {
            m_infoController->setValue(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST, -1);
            m_infoController->setValue(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST, -1);
            qCWarning(LG_BOILER_LOGIC) << "No weather forecast available";
        }

        double currentTankTemp = m_tempController->value(EnumsDeclarations::TEMPS_TANK).toDouble();
        int minTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP).toInt();
        int sunExpSubstract = m_settingsController->value(EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT).toInt();
        QString bs = m_settingsController->value(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE).toString();
        targetTemp = ForecastUtils::calculateTargetTemp(QTime::currentTime().hour(), bs, minTemp, sunExpected, sunExpSubstract);

        qCDebug(LG_BOILER_LOGIC) << "Target temp" << targetTemp << minTemp;

        boilerOn = currentTankTemp < targetTemp;
    } else {
        m_infoController->setValue(EnumsDeclarations::INFOS_AVG_TEMP_FORECAST, -1);
        m_infoController->setValue(EnumsDeclarations::INFOS_AVG_CLOUDS_FORECAST, -1);
        qCWarning(LG_BOILER_LOGIC) << "Tank temp is not valid";
    }


    m_infoController->setValue(EnumsDeclarations::INFOS_BOILER_TARGET_TEMP, targetTemp);
    m_infoController->setValue(EnumsDeclarations::INFOS_SUN_EXPECTED, sunExpectedVB);

    m_relayController->setValue(EnumsDeclarations::RELAYS_BOILER, boilerOn, true, true);
}



void BoilerLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_BOILER_LOGIC) << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
