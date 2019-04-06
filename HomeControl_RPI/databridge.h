#ifndef DATABRIDGE_H
#define DATABRIDGE_H

#include <QObject>
#include "controllerlistmodel.h"
#include "controllermanager.h"
#include "appconfiguration.h"
#include "tempcontroller.h"
#include "errorcontroller.h"
#include "errorcontrollerlistmodel.h"

class DataBridge : public QObject
{
    Q_OBJECT
public:
    explicit DataBridge(AppConfiguration *appConfig, QObject *parent = nullptr);

    Q_PROPERTY(ControllerListModel* tempController READ tempListModelController NOTIFY tempControllerChanged)
    Q_PROPERTY(ErrorControllerListModel* errorController READ errorListModelController NOTIFY errorControllerChanged)

    ControllerListModel *tempListModelController();
    ErrorControllerListModel *errorListModelController();

private:
    AppConfiguration* m_appConfig;

    ControllerManager m_controllerManager;
    TempController m_tempController;
    ErrorController m_errorController;

    ControllerListModel *m_tempListModelController;
    ErrorControllerListModel *m_errorListModelController;

private slots:
    void onMqttConnected();

signals:
    void tempControllerChanged();
    void errorControllerChanged();


public slots:
};

#endif // DATABRIDGE_H
