#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QMqttClient>
#include "appconfiguration.h"

class ControllerManager;        // fwd dcl

class ControllerBase : public QObject
{
    Q_OBJECT
public:
    explicit ControllerBase(QObject *parent = nullptr);

    virtual QString getName() = 0;

    virtual QStringList getTopicPath() = 0;

    virtual QStringList getLabelList() = 0;

    virtual QVariant::Type getValueType(int index = -1) = 0;

    void init(ControllerManager* parent, AppConfiguration* appConfig, QMqttClient *mqttClient);

    AppConfiguration* getConfig();

    void publish(int index);

    QList<QVariant> getValues();

    void setValue(int index, QVariant value);

    QString getLabel(int index);

    void clearValue(int index);

protected:
    QList<QVariant> m_values;
    QStringList m_topicPath;
    QStringList m_labels;
    ControllerManager* m_parent;

    QString m_topicName;

    virtual void onInit();

    virtual void onMqttUnknownMessageReceived(QStringList topicPath, QByteArray data);

    virtual void onUnmappedMqttDoubleReceived(QStringList topicPath, double value);
    virtual void onUnmappedMqttIntReceived(QStringList topicPath, int value);
    virtual void onUnmappedMqttStringReceived(QStringList topicPath, QString value);
    virtual void onUnmappedMqttBoolReceived(QStringList topicPath, bool value);

    virtual void onValueChanged(int index, QVariant value);

private:
    AppConfiguration* m_appConfig;
    QMqttClient *m_mqttClient;

    QMqttSubscription* m_topicSub;
    QMqttSubscription* m_bcSub;

signals:
    void valueChanged(int index, QVariant value);

public slots:

private slots:
    void onMqttConnected();
    void onMqttDisconnected();

    void _onMqttMessageReceived(QMqttMessage msg);

};

#endif // CONTROLLERBASE_H
