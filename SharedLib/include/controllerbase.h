#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QMqttClient>
#include "appconfiguration.h"

class ControllerManager;        // fwd dcl

#define CONVERT_LABEL_LIST(LIST_NAME) QStringList returnList;for(QString t:LIST_NAME){returnList << t;}return returnList;

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

    void broadcastValues();

    QList<QVariant> values();

    QVariant value(int index);

    void setValue(int index, QVariant value, bool sendSet = false);

    QString getLabel(int index);

    void clearValue(int index);

    static QVariant parsePayload(QByteArray payload);

protected:
    QList<QVariant> m_values;
    QStringList m_topicPath;
    QStringList m_labels;
    ControllerManager* m_parent;

    QString m_topicName;

    virtual bool hasSetSupport() {
        return false;
    }

    virtual void onInit();

    virtual void onMqttUnknownMessageReceived(QStringList topicPath, QByteArray data);

    virtual void onUnmappedMqttValueReceived(QStringList topicPath, QVariant value);

    virtual void onConnectedChanged(bool connected);

    virtual void onSetReceived(int index, QVariant value);

    virtual void onValueChanged(int index, QVariant value);

private:
    AppConfiguration* m_appConfig;
    QMqttClient *m_mqttClient;

    QMqttSubscription* m_topicValSub;
    QMqttSubscription* m_topicSetSub;
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
