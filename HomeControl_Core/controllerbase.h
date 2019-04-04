#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QMqttClient>
#include "appconfiguration.h"

class ControllerBase : public QObject
{
    Q_OBJECT
public:
    enum MQTT_MODE {
        MQTT_MODE_NONE,
        MQTT_MODE_VAL,
        MQTT_MODE_SET
    };

    explicit ControllerBase(AppConfiguration* appConfig, QObject *parent = nullptr);

    virtual QString getName() = 0;

    virtual QStringList getTopicPath() = 0;

    void init();

    AppConfiguration* getConfig();

    void publish(int index);

protected:
    QList<QVariant> m_values;
    QStringList m_topicPath;
    QString m_topicName;

    static QStringList buildPath(QStringList paths, MQTT_MODE mode = MQTT_MODE_NONE, bool addWildcard = false);
    static QStringList cleanPath(QStringList paths);

    virtual void onInit();

    virtual void onMqttConnected();
    virtual void onMqttUnknownMessageReceived(QStringList topicPath, QByteArray data);

    virtual void onUnmappedMqttDoubleReceived(QStringList topicPath, double value);
    virtual void onUnmappedMqttIntReceived(QStringList topicPath, int value);
    virtual void onUnmappedMqttStringReceived(QStringList topicPath, QString value);
    virtual void onUnmappedMqttBoolReceived(QStringList topicPath, bool value);

    virtual void onValueChanged(int index, QVariant value);

    static QByteArray getPayload(QVariant value);

private:
    AppConfiguration* m_appConfig;

    QMqttClient m_mqttClient;

signals:

public slots:

private slots:
    void _onMqttConnected();
    void _onMqttMessageReceived(QMqttMessage msg);

};

#endif // CONTROLLERBASE_H
