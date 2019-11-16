#ifndef THINGSPEAKLOGGER_H
#define THINGSPEAKLOGGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QQueue>
#include <QMutex>
#include <QLoggingCategory>

#ifndef QT_NO_SSL
    #include <QSslError>
#else
    #warning No SSL Support
#endif

#include "appconfiguration.h"
#include "controller/controllermanager.h"
#include "controller/logiccontroller.h"

#include "controller/tempcontroller.h"
#include "controller/relaycontroller.h"

Q_DECLARE_LOGGING_CATEGORY(LG_THINGSPEAK_LOGGERS)

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

    QQueue<QString> m_queue;

    QString buildQuery(int index, QString value, QString apiKey);

    void executeRequest(QString query);

    void _executeRequest(QString query);

    QTimer m_RequestTimer;

private slots:
    void onTempValueChanged(int index, QVariant value);
    void onRelayValueChanged(int index, QVariant value);

    void onFinished(QNetworkReply *reply);
#ifndef QT_NO_SSL
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

    void checkNextRequest();

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);

signals:

};

#endif // THINGSPEAKLOGGER_H
