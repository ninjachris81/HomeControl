#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QMqttClient>
#include "appconfiguration.h"

class ControllerBase : public QObject
{
    Q_OBJECT
public:
    explicit ControllerBase(AppConfiguration* appConfig, QObject *parent = nullptr);

    virtual QString getName() = 0;

    virtual QMqttTopicFilter getTopicFilter();

    void init();

    AppConfiguration* getConfig();

protected:
    QMap<QString, QVariant> m_valueMap;

    virtual void onInit();

    virtual void onMqttConnected();
    virtual void onMqttUnknownMessageReceived(QString topic, QByteArray data);

    virtual void onMqttDoubleReceived(QString topic, double value);
    virtual void onMqttIntReceived(QString topic, int value);
    virtual void onMqttStringReceived(QString topic, QString value);
    virtual void onMqttBoolReceived(QString topic, bool value);

    virtual void onValueChanged(QString topic, QVariant value);

    static QByteArray getPayload(QString topic, QVariant value);

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
