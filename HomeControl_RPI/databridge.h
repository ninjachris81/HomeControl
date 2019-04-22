#ifndef DATABRIDGE_H
#define DATABRIDGE_H

#include <QObject>
#include "listmodel/controllerlistmodel.h"
#include "controller/controllermanager.h"
#include "appconfiguration.h"
#include "controller/tempcontroller.h"
#include "controller/errorcontroller.h"
#include "listmodel/errorcontrollerlistmodel.h"
#include "controller/relaycontroller.h"
#include "listmodel/relaycontrollerlistmodel.h"
#include "controller/settingscontroller.h"
#include "listmodel/settingscontrollerlistmodel.h"

#include "wrapper/settingswrapper.h"

class DataBridge : public QObject
{
    Q_OBJECT
public:
    explicit DataBridge(AppConfiguration *appConfig, QObject *parent = nullptr);

    Q_PROPERTY(ControllerListModel* tempControllerModel READ tempListModelController NOTIFY tempControllerModelChanged)
    Q_PROPERTY(ErrorControllerListModel* errorControllerModel READ errorListModelController NOTIFY errorControllerModelChanged)
    Q_PROPERTY(RelayControllerListModel* relayControllerModel READ relayListModelController NOTIFY relayControllerModelChanged)
    Q_PROPERTY(SettingsControllerListModel* settingsControllerModel READ settingsListModelController NOTIFY settingsControllerModelChanged)

    Q_PROPERTY(SettingsWrapper* settings READ settingsControllerWrapper NOTIFY settingsControllerChanged)

    ControllerListModel *tempListModelController();
    ErrorControllerListModel *errorListModelController();
    RelayControllerListModel *relayListModelController();
    SettingsControllerListModel *settingsListModelController();

    SettingsWrapper *settingsControllerWrapper();

private:
    AppConfiguration* m_appConfig;

    ControllerManager m_controllerManager;
    TempController m_tempController;
    ErrorController m_errorController;
    RelayController m_relayController;
    SettingsController m_settingsController;

    ControllerListModel *m_tempListModelController;
    ErrorControllerListModel *m_errorListModelController;
    RelayControllerListModel *m_relayListModelController;
    SettingsControllerListModel *m_settingsListModelController;

    SettingsWrapper *m_settingsControllerWrapper;

private slots:
    void onMqttConnected();

signals:
    void tempControllerModelChanged();
    void errorControllerModelChanged();
    void relayControllerModelChanged();
    void settingsControllerModelChanged();

    void settingsControllerChanged();


public slots:
};

#endif // DATABRIDGE_H
