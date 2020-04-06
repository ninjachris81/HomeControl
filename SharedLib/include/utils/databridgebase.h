#ifndef DATABRIDGEBASE_H
#define DATABRIDGEBASE_H

#include <QObject>
#include "listmodel/controllerlistmodel.h"
#include "utils/appconfiguration.h"
#include "controller/controllermanager.h"
#include "controller/tempcontroller.h"
#include "controller/logcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/brightnesscontroller.h"
#include "controller/switchcontroller.h"
#include "controller/infocontroller.h"
#include "controller/dataloggercontroller.h"

#include "wrapper/settingswrapper.h"
#include "wrapper/logwrapper.h"
#include "wrapper/infowrapper.h"
#include "wrapper/dataloggerwrapper.h"

#include "utils/weatherforecastmanager.h"

class DataBridgeBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBridgeBase(AppConfiguration *appConfig, QObject *parent = nullptr);

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

    // controllers
    Q_PROPERTY(ControllerListModel* tempControllerModel READ tempListModelController NOTIFY tempControllerModelChanged)
    Q_PROPERTY(ControllerListModel* logControllerModel READ logListModelController NOTIFY logControllerModelChanged)
    Q_PROPERTY(ControllerListModel* relayControllerModel READ relayListModelController NOTIFY relayControllerModelChanged)
    Q_PROPERTY(ControllerListModel* settingsControllerModel READ settingsListModelController NOTIFY settingsControllerModelChanged)
    Q_PROPERTY(ControllerListModel* brightnessControllerModel READ brightnessListModelController NOTIFY brightnessControllerModelChanged)
    Q_PROPERTY(ControllerListModel* switchControllerModel READ switchListModelController NOTIFY switchControllerModelChanged)

    // wrappers
    Q_PROPERTY(SettingsWrapper* settings READ settingsControllerWrapper NOTIFY settingsControllerChanged)
    Q_PROPERTY(LogWrapper* logs READ logControllerWrapper NOTIFY logControllerChanged)
    Q_PROPERTY(InfoWrapper* infos READ infoControllerWrapper NOTIFY infoControllerChanged)
    Q_PROPERTY(DataLoggerWrapper* dataLogger READ dataLoggerControllerWrapper NOTIFY dataLoggerControllerChanged)

    Q_PROPERTY(QVariantMap appConfigModel READ appConfigModel NOTIFY appConfigModelChanged)
    Q_PROPERTY(WeatherForecastManager* wfcManager READ wfcManager NOTIFY wfcManagerChanged)

    bool isConnected();

    ControllerListModel *tempListModelController();
    ControllerListModel *logListModelController();
    ControllerListModel *relayListModelController();
    ControllerListModel *settingsListModelController();
    ControllerListModel *brightnessListModelController();
    ControllerListModel *switchListModelController();

    SettingsWrapper *settingsControllerWrapper();
    LogWrapper *logControllerWrapper();
    InfoWrapper *infoControllerWrapper();
    DataLoggerWrapper *dataLoggerControllerWrapper();

    QVariantMap appConfigModel();

    WeatherForecastManager* wfcManager() { return m_wfcManager; }

    Q_INVOKABLE QVariant getAppConfig(QString key, QVariant defaultValue);
    Q_INVOKABLE void setAppConfig(QString key, QVariant value);

private:
    AppConfiguration* m_appConfig;

    bool m_isConnected = false;

    ControllerManager m_controllerManager;
    TempController m_tempController;
    LogController m_logController;
    RelayController m_relayController;
    SettingsController m_settingsController;
    BrightnessController m_brightnessController;
    SwitchController m_switchController;
    InfoController m_infoController;
    DataLoggerController m_dataLoggerController;

    ControllerListModel *m_tempListModelController;
    ControllerListModel *m_logListModelController;
    ControllerListModel *m_relayListModelController;
    ControllerListModel *m_settingsListModelController;
    ControllerListModel *m_brightnessListModelController;
    ControllerListModel *m_switchListModelController;

    SettingsWrapper *m_settingsControllerWrapper;
    LogWrapper *m_logControllerWrapper;
    InfoWrapper *m_infoControllerWrapper;
    DataLoggerWrapper *m_dataLoggerWrapper;

    WeatherForecastManager *m_wfcManager;

private slots:
    void onMqttConnected();

signals:
    void isConnectedChanged();

    void tempControllerModelChanged();
    void logControllerModelChanged();
    void relayControllerModelChanged();
    void settingsControllerModelChanged();
    void brightnessControllerModelChanged();
    void switchControllerModelChanged();

    void settingsControllerChanged();
    void logControllerChanged();
    void infoControllerChanged();
    void dataLoggerControllerChanged();

    void appConfigModelChanged();

    void wfcManagerChanged();

public slots:
};

#endif // DATABRIDGEBASE_H
