#ifndef THINGSPEAKLOGGER_H
#define THINGSPEAKLOGGER_H

#include <QObject>
#include <QNetworkAccessManager>

#include "appconfiguration.h"
#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"
#include "controller/tempcontroller.h"

class ThingSpeakLogger : public LogicController
{
    Q_OBJECT
public:
    explicit ThingSpeakLogger(ControllerManager *controllerManager, AppConfiguration *appConfig, QObject *parent = nullptr);

private:
    QNetworkAccessManager m_nam;
    TempController* m_tempController;

    QString m_apiKey;

private slots:
    void onTempValueChanged(int index, QVariant value);

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // THINGSPEAKLOGGER_H
