#ifndef THINGSPEAKLOGGER_H
#define THINGSPEAKLOGGER_H

#include <QObject>
#include <QNetworkAccessManager>

#include "appconfiguration.h"
#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"

#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"

class ThingSpeakLogger : public LogicController
{
    Q_OBJECT
public:
    explicit ThingSpeakLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent = nullptr);

private:
    QNetworkAccessManager m_nam;

    TempController* m_tempController;
    RelayController* m_relayController;

    QString m_apiKeyTemp;
    QString m_apiKeyRelay;

    void executeRequest(QString query, QString apiKey);

private slots:
    void onTempValueChanged(int index, QVariant value);
    void onRelayValueChanged(int index, QVariant value);

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // THINGSPEAKLOGGER_H
