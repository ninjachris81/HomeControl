#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QMqttClient>
#include <QDateTime>
#include <QTimer>

#include "../appconfiguration.h"
#include "../constants_qt.h"

class ControllerManager;        // fwd dcl

#define CONVERT_LABEL_LIST(LIST_NAME) QStringList returnList;for(QString t:LIST_NAME){returnList << t;}return returnList;

class ControllerBase : public QObject
{
    Q_OBJECT
public:
    enum VALUE_OWNER_MODE {
        VALUE_OWNER_SERVER,
        VALUE_OWNER_CLIENT,
        VALUE_OWNER_DEFAULT = VALUE_OWNER_CLIENT
    };

    struct ValueStruct {
        qint64 lifeTime = LIFETIME_UNLIMITED;
        qint64 lastUpdate;
        QVariant value;
        bool wasValid = false;

        bool isValid() {
            if (lifeTime==LIFETIME_UNLIMITED) {
                wasValid = true;
            } else if (lastUpdate==0) {
                wasValid = false;
            } else {
                wasValid = QDateTime::currentMSecsSinceEpoch()<lastUpdate+lifeTime;
            }
            return wasValid;
        }

        bool compareTo(QVariant &val) {
            return value==val;
        }

        void updateValue(QVariant val) {
            value = val;
            lastUpdate = QDateTime::currentMSecsSinceEpoch();
        }
    };

    explicit ControllerBase(QObject *parent = nullptr);

    virtual QString getName() = 0;

    virtual QStringList getTopicPath() = 0;

    virtual QStringList getLabelList() = 0;

    virtual QList<QVariant::Type> getValueTypes();

    virtual QVariant::Type getDefaultValueType();

    virtual qint64 getValueLifetime(int index = -1) = 0;

    virtual QString getEnumName() = 0;

    virtual bool isValueOwner(int index = -1);

    virtual void broadcastValues();

    void init(ControllerManager* parent, AppConfiguration* appConfig, QMqttClient *mqttClient);

    AppConfiguration* getConfig();

    void publish(int index);

    void publishCmd(EnumsDeclarations::MQTT_CMDS cmd);

    QList<ValueStruct> values();

    QVariant value(int index);

    bool valueIsValid(int index);

    QVariant::Type getValueType(int index = -1);

    void setValue(int index, QVariant value, bool sendSet = false, bool ignoreCompare = false);

    QString getLabel(int index);

    void clearValue(int index);

    static QVariant parsePayload(QByteArray payload);

    void setMode(VALUE_OWNER_MODE thisMode);

protected:
    VALUE_OWNER_MODE m_mode = VALUE_OWNER_DEFAULT;
    QList<ValueStruct> m_values;
    QStringList m_topicPath;
    QStringList m_labels;
    ControllerManager* m_parent;

    QList<QVariant::Type> m_TypeCache;

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

    virtual void onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd);

protected slots:

private:
    AppConfiguration* m_appConfig;
    QMqttClient *m_mqttClient;

    QMqttSubscription* m_topicValSub;
    QMqttSubscription* m_topicSetSub;
    QMqttSubscription* m_bcSub;

    QTimer m_validityTimer;

signals:
    void valueChanged(int index, QVariant value);
    void valueValidChanged(int index);

public slots:

private slots:
    void onMqttConnected();
    void onMqttDisconnected();

    void _onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd);

    void _onMqttMessageReceived(QMqttMessage msg);

    void onCheckValidity();

};

#endif // CONTROLLERBASE_H
