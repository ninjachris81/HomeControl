#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H


#include <QObject>
#include <QMap>
#include "controllerbase.h"
#include "utils/appconfiguration.h"
#include "constants_qt.h"

class ControllerManager : public QObject
{
    Q_OBJECT
public:
    enum MQTT_MODE {
        MQTT_MODE_NONE,
        MQTT_MODE_VAL,
        MQTT_MODE_SET,
        MQTT_MODE_TRE
    };

    enum MQTT_BROADCAST_TYPE {
        MQTT_BC_ALL,
        MQTT_BC_TEMPS,
        MQTT_BC_RELAYS
    };

    explicit ControllerManager(bool isServer = false, QObject *parent = nullptr);

    void init(AppConfiguration* appConfig);

    void registerController(ControllerBase *controller);

    ControllerBase* getController(QString name);

    static QString getBroadcastValue(MQTT_BROADCAST_TYPE type);

    void publishBC(MQTT_BROADCAST_TYPE type);

    static QStringList buildPath(QStringList paths, MQTT_MODE mode = MQTT_MODE_NONE, int index = -1, bool addWildcard = false);
    static QStringList cleanPath(QStringList paths);

    static QByteArray getPayload(QVariant value);

    void publish(QStringList path, QVariant value);

    void publishCmd(EnumsDeclarations::MQTT_CMDS cmd);

    bool isServer();

    AppConfiguration* appConfig() {
        return m_appConfig;
    }

private:
    QMap<QString, ControllerBase*> m_controllers;
    AppConfiguration* m_appConfig;

    QMqttClient m_mqttClient;

    QMqttSubscription* m_cmdSub;

    bool m_isServer = false;

    void _onMqttConnected();
    void _onMqttDisconnected();

private slots:
    void _onMqttStateChanged(QMqttClient::ClientState state);
    void _onMqttError(QMqttClient::ClientError error);

    void _onMqttCmdReceived(QMqttMessage msg);

signals:
    void mqttConnected();
    void mqttDisconnected();

    void mqttCmdReceived(EnumsDeclarations::MQTT_CMDS cmd);

public slots:
};

#endif // CONTROLLERMANAGER_H
