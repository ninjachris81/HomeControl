#ifndef DATABRIDGE_H
#define DATABRIDGE_H

#include <QObject>
#include "listmodel/controllerlistmodel.h"
#include "utils/appconfiguration.h"
#include "controller/controllermanager.h"
#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"
#include "controller/settingscontroller.h"
#include "controller/brightnesscontroller.h"
#include "controller/switchcontroller.h"
#include "controller/infocontroller.h"

#include "wrapper/settingswrapper.h"
#include "wrapper/infowrapper.h"

class DataBridge : public QObject
{
    Q_OBJECT
public:
    explicit DataBridge(AppConfiguration *appConfig, QObject *parent = nullptr);

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

    Q_PROPERTY(ControllerListModel* tempControllerModel READ tempListModelController NOTIFY tempControllerModelChanged)
    Q_PROPERTY(ControllerListModel* relayControllerModel READ relayListModelController NOTIFY relayControllerModelChanged)
    Q_PROPERTY(ControllerListModel* settingsControllerModel READ settingsListModelController NOTIFY settingsControllerModelChanged)
    Q_PROPERTY(ControllerListModel* brightnessControllerModel READ brightnessListModelController NOTIFY brightnessControllerModelChanged)
    Q_PROPERTY(ControllerListModel* switchControllerModel READ switchListModelController NOTIFY switchControllerModelChanged)

    Q_PROPERTY(SettingsWrapper* settings READ settingsControllerWrapper NOTIFY settingsControllerChanged)
    Q_PROPERTY(InfoWrapper* infos READ infoControllerWrapper NOTIFY infoControllerChanged)

    Q_PROPERTY(QVariantMap appConfigModel READ appConfigModel NOTIFY appConfigModelChanged)

    bool isConnected();

    ControllerListModel *tempListModelController();
    ControllerListModel *relayListModelController();
    ControllerListModel *settingsListModelController();
    ControllerListModel *brightnessListModelController();
    ControllerListModel *switchListModelController();

    SettingsWrapper *settingsControllerWrapper();
    InfoWrapper *infoControllerWrapper();

    QVariantMap appConfigModel();

    Q_INVOKABLE QVariant getAppConfig(QString key, QVariant defaultValue);
    Q_INVOKABLE void setAppConfig(QString key, QVariant value);

private:
    AppConfiguration* m_appConfig;

    bool m_isConnected = false;

    ControllerManager m_controllerManager;
    TempController m_tempController;
    RelayController m_relayController;
    SettingsController m_settingsController;
    BrightnessController m_brightnessController;
    SwitchController m_switchController;
    InfoController m_infoController;

    ControllerListModel *m_tempListModelController;
    ControllerListModel *m_relayListModelController;
    ControllerListModel *m_settingsListModelController;
    ControllerListModel *m_brightnessListModelController;
    ControllerListModel *m_switchListModelController;

    SettingsWrapper *m_settingsControllerWrapper;
    InfoWrapper *m_infoControllerWrapper;

private slots:
    void onMqttConnected();

signals:
    void isConnectedChanged();

    void tempControllerModelChanged();
    void relayControllerModelChanged();
    void settingsControllerModelChanged();
    void brightnessControllerModelChanged();
    void switchControllerModelChanged();

    void settingsControllerChanged();
    void infoControllerChanged();

    void appConfigModelChanged();

public slots:
};

#endif // DATABRIDGE_H
