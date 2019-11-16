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
    enum CONTROLLER_TYPE {
        BRIGHTNESS_CONTROLLER,
        INFO_CONTROLLER,
        LOG_CONTROLLER,
        RELAY_CONTROLLER,
        SETTINGS_CONTROLLER,
        SWITCH_CONTROLLER,
        TEMP_CONTROLLER
    };

    enum VALUE_OWNER_MODE {
        VALUE_OWNER_SERVER,
        VALUE_OWNER_CLIENT,
        VALUE_OWNER_DEFAULT = VALUE_OWNER_CLIENT
    };

    enum VALUE_BC_INTERVAL {
        VALUE_BC_NONE = 0,
        VALUE_BC_FAST=5000,
        VALUE_BC_SLOW=20000,
        VALUE_BC_FASTEST = VALUE_BC_FAST
    };

    enum VALUE_TREND {
        VALUE_TREND_NONE,
        VALUE_TREND_CONSTANT,
        VALUE_TREND_INCREASING,
        VALUE_TREND_DECREASING
    };

    enum VALUE_TREND_TIMEOUT {
        VALUE_TT_NONE = 0,
        VALUE_TT_FAST = 30000,
        VALUE_TT_MID = 120000,
        VALUE_TT_SLOW = 600000
    };

    struct ValueStruct {
        QVariant value;

        qint64 _lifeTime = LIFETIME_UNLIMITED;
        qint64 _lastUpdate = 0;
        bool _wasValid = false;
        VALUE_TREND _trend = VALUE_TREND_NONE;
        qint64 _lastTrendUpdate = 0;
        qint64 _trendLifeTime = VALUE_TT_NONE;

        bool isValid() {
            if (_lifeTime==LIFETIME_UNLIMITED) {
                _wasValid = true;
            } else if (_lastUpdate==0) {
                _wasValid = false;
            } else {
                _wasValid = QDateTime::currentMSecsSinceEpoch()<_lastUpdate+_lifeTime;
            }
            return _wasValid;
        }

        bool compareTo(QVariant &val) {
            return value==val;
        }

        VALUE_TREND valueTrend() {
            if (_trendLifeTime>0) {
                if (QDateTime::currentMSecsSinceEpoch()<_lastTrendUpdate+_trendLifeTime) {
                    _trend = VALUE_TREND_NONE;
                }
            }

            return _trend;
        }

        void updateValue(QVariant val) {
            switch(value.type()) {
            case QVariant::Int:
            case QVariant::UInt:
            case QVariant::Double:
            case QVariant::LongLong:
            case QVariant::ULongLong:
                if (val>value) {
                    _trend = VALUE_TREND_INCREASING;
                } else if (val<value) {
                    _trend = VALUE_TREND_DECREASING;
                } else {
                    _trend = VALUE_TREND_CONSTANT;
                }
                _lastTrendUpdate = QDateTime::currentMSecsSinceEpoch();
                break;
            default:
                break;
            }

            value = val;
            _lastUpdate = QDateTime::currentMSecsSinceEpoch();
        }
    };

    explicit ControllerBase(QObject *parent = nullptr);

    virtual QString getName() = 0;

    virtual CONTROLLER_TYPE getType() = 0;

    virtual QStringList getTopicPath() = 0;

    virtual QStringList getLabelList() = 0;

    virtual QList<QVariant::Type> getValueTypes();

    virtual QVariant::Type getDefaultValueType();

    virtual qint64 getValueLifetime(int index = -1) = 0;

    virtual qint64 getValueTrendLifetime(int index = -1);

    virtual VALUE_BC_INTERVAL getValueBCInterval(int index=-1);

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

    QTimer m_valueUpdateTimer;
    QTimer m_valueBCTimer;

signals:
    void valueChanged(int index, QVariant value);
    void valueValidChanged(int index);
    void valueTrendChanged(int index);

public slots:

private slots:
    void onMqttConnected();
    void onMqttDisconnected();

    void _onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd);

    void _onMqttMessageReceived(QMqttMessage msg);

    void onCheckValue();

    void onCheckBroadcasts();

};

#endif // CONTROLLERBASE_H
