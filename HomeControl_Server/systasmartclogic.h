#ifndef SYSTASMARTCLOGIC_H
#define SYSTASMARTCLOGIC_H

#include <QObject>
#include <QLoggingCategory>

#include "controller/logiccontroller.h"
#include "controller/ssccontroller.h"

#include "systasmartcclient.h"
#include "utils/appconfiguration.h"

Q_DECLARE_LOGGING_CATEGORY(LG_SYSTA_SMART_C_LOGIC)

class SystaSmartCLogic : public LogicController
{
    Q_OBJECT

public:
    explicit SystaSmartCLogic(ControllerManager *controllerManager, SystaSmartCClient *client, AppConfiguration &appConfig, QObject *parent = nullptr);

private:
    SSCController* m_sscController;
    SystaSmartCClient *m_client;

private slots:
    void onReceivedVariable(EnumsDeclarations::MQTT_SSC_VARIABLES variable, QVariant value);

signals:

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // SYSTASMARTCLOGIC_H
