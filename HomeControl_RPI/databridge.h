#ifndef DATABRIDGE_H
#define DATABRIDGE_H

#include <QObject>
#include "listmodel/controllerlistmodel.h"
#include "controller/controllermanager.h"
#include "appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/logcontroller.h"
#include "listmodel/logcontrollerlistmodel.h"
#include "controller/relaycontroller.h"
#include "listmodel/relaycontrollerlistmodel.h"
#include "controller/settingscontroller.h"
#include "listmodel/settingscontrollerlistmodel.h"
#include "sqllistmodel/logsqllistmodel.h"

#include "wrapper/settingswrapper.h"

class DataBridge : public QObject
{
    Q_OBJECT
public:
    explicit DataBridge(AppConfiguration *appConfig, QObject *parent = nullptr);

    Q_PROPERTY(ControllerListModel* tempControllerModel READ tempListModelController NOTIFY tempControllerModelChanged)
    Q_PROPERTY(LogControllerListModel* logControllerModel READ logListModelController NOTIFY logControllerModelChanged)
    Q_PROPERTY(RelayControllerListModel* relayControllerModel READ relayListModelController NOTIFY relayControllerModelChanged)
    Q_PROPERTY(SettingsControllerListModel* settingsControllerModel READ settingsListModelController NOTIFY settingsControllerModelChanged)

    Q_PROPERTY(SettingsWrapper* settings READ settingsControllerWrapper NOTIFY settingsControllerChanged)
    Q_PROPERTY(LogSqlListModel* logListModel READ logListModel NOTIFY logListModelChanged)

    ControllerListModel *tempListModelController();
    LogControllerListModel *logListModelController();
    RelayControllerListModel *relayListModelController();
    SettingsControllerListModel *settingsListModelController();

    SettingsWrapper *settingsControllerWrapper();

    LogSqlListModel *logListModel();

private:
    AppConfiguration* m_appConfig;

    ControllerManager m_controllerManager;
    TempController m_tempController;
    LogController m_logController;
    RelayController m_relayController;
    SettingsController m_settingsController;

    ControllerListModel *m_tempListModelController;
    LogControllerListModel *m_logListModelController;
    RelayControllerListModel *m_relayListModelController;
    SettingsControllerListModel *m_settingsListModelController;

    SettingsWrapper *m_settingsControllerWrapper;

    LogSqlListModel *m_logListModel;

private slots:
    void onMqttConnected();

signals:
    void tempControllerModelChanged();
    void logControllerModelChanged();
    void relayControllerModelChanged();
    void settingsControllerModelChanged();

    void settingsControllerChanged();

    void logListModelChanged();


public slots:
};

#endif // DATABRIDGE_H
