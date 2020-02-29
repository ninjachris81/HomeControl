#include "boilerlogic.h"
#include <QDateTime>

Q_LOGGING_CATEGORY(LG_BOILER_LOGIC, "BoilerLogic");

BoilerLogic::BoilerLogic(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent) : LogicController(controllerManager, BOILER_LOGIC_INTERVAL, parent)
{
    m_tempController = static_cast<TempController*>(controllerManager->getController(TempController::CONTROLLER_NAME));
    m_relayController = static_cast<RelayController*>(controllerManager->getController(RelayController::CONTROLLER_NAME));
    m_settingsController = static_cast<SettingsController*>(controllerManager->getController(SettingsController::CONTROLLER_NAME));
    m_infoController = static_cast<InfoController*>(controllerManager->getController(InfoController::CONTROLLER_NAME));

    m_wfcManager = new WeatherForecastManager(appConfig->getString("WFC_API_KEY", ""));
    m_wfcTimer.setInterval(30 * 60000);     // every 30 minutes
    connect(&m_wfcTimer, &QTimer::timeout, this, &BoilerLogic::onRefreshWFC);
    m_wfcTimer.start();
    onRefreshWFC();
}

BoilerLogic::~BoilerLogic() {
    delete m_wfcManager;
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

            sunExpected = avgTempTomorrow>=m_settingsController->value(EnumsDeclarations::SETTINGS_TEMP_EXP_THRESHOLD).toInt() && avgCloudsTomorrow<=m_settingsController->value(EnumsDeclarations::SETTINGS_CLOUDS_EXP_THRESHOLD).toInt();
            qCDebug(LG_BOILER_LOGIC) << "Sun expected" << sunExpected;

            sunExpectedVB = sunExpected ? EnumsDeclarations::BOOL_TRUE : EnumsDeclarations::BOOL_FALSE;
        } else {
            qCWarning(LG_BOILER_LOGIC) << "No weather forecast available";
        }

        double currentTankTemp = m_tempController->value(EnumsDeclarations::TEMPS_TANK).toDouble();
        int minTemp = m_settingsController->value(EnumsDeclarations::SETTINGS_TANK_MIN_TEMP).toInt();
        int sunExpSubstract = m_settingsController->value(EnumsDeclarations::SETTINGS_SUN_EXP_SUBSTRACT).toInt();
        int scheduledTempDelta = getScheduledTempDelta();

        targetTemp = minTemp + scheduledTempDelta + (sunExpected ? -sunExpSubstract : 0);

        qCDebug(LG_BOILER_LOGIC) << "Target temp" << targetTemp << minTemp << scheduledTempDelta;

        boilerOn = currentTankTemp < targetTemp;
    } else {
        qCWarning(LG_BOILER_LOGIC) << "Tank temp is not valid";
    }


    boilerOn = false;       // TODO REMOVE ME !!!!!!

    m_infoController->setValue(EnumsDeclarations::INFOS_BOILER_TARGET_TEMP, targetTemp);
    m_infoController->setValue(EnumsDeclarations::INFOS_SUN_EXPECTED, sunExpectedVB);

    m_relayController->setValue(EnumsDeclarations::RELAYS_BOILER, boilerOn, true);
}

int BoilerLogic::getScheduledTempDelta() {
    qCDebug(LG_BOILER_LOGIC) << Q_FUNC_INFO;

    QString bs = m_settingsController->value(EnumsDeclarations::SETTINGS_BOILER_SCHEDULE).toString();

    if (!bs.isEmpty()) {
        QStringList hours = bs.split(" ", QString::SkipEmptyParts);
        if (hours.count()==24) {
            return hours.at(QTime::currentTime().hour()).toInt();
        } else {
            qCWarning(LG_BOILER_LOGIC) << "Invalid boiler schedule" << bs << hours.count();
        }
    } else {
        qCWarning(LG_BOILER_LOGIC) << "No boiler schedule set";
    }

    return 0;
}

void BoilerLogic::onRefreshWFC() {
    qCDebug(LG_BOILER_LOGIC) << Q_FUNC_INFO;
    m_wfcManager->requestForecast();
}

void BoilerLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_BOILER_LOGIC) << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}
