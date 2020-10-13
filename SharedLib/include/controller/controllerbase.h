#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QMqttClient>
#include <QDateTime>
#include <QTimer>
#include <QMutex>

#include "utils/appconfiguration.h"
#include "constants_qt.h"

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
        TEMP_CONTROLLER,
        DATA_LOG_CONTROLLER,
        HUMIDITY_CONTROLLER,
        CURRENT_CONTROLLER,
        EXTERNAL_PV_CONTROLLER = 100        // starting from 100
    };

    enum VALUE_OWNER_MODE {
        VALUE_OWNER_SERVER,
        VALUE_OWNER_CLIENT,
        VALUE_OWNER_EXTERNAL_SENSOR,
        VALUE_OWNER_DEFAULT = VALUE_OWNER_SERVER
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
        QVariant _value;
        qint64 _lifeTime = LIFETIME_UNLIMITED;
        qint64 _lastUpdate = 0;
        bool _wasValid = false;
        VALUE_TREND _trend = VALUE_TREND_NONE;
        qint64 _lastTrendUpdate = 0;
        qint64 _trendLifeTime = VALUE_TT_NONE;
        bool _isFirstUpdate = true;

        bool isValid() {
            if (_lifeTime==LIFETIME_ALWAYS_VALID) {
                _wasValid = true;
            } else if (_lifeTime==LIFETIME_UNLIMITED) {
                _wasValid = _lastUpdate!=0;
            } else if (_lastUpdate==0) {
                _wasValid = false;
            } else {
                _wasValid = QDateTime::currentMSecsSinceEpoch()<_lastUpdate+_lifeTime;
            }
            return _wasValid;
        }

        bool compareTo(QVariant &val) {
            return _value==val;
        }

        // client
        bool valueTrendReceived(int trend) {
            _lastTrendUpdate = QDateTime::currentMSecsSinceEpoch();
            VALUE_TREND newTrend = static_cast<VALUE_TREND>(trend);

            qDebug() << Q_FUNC_INFO << trend << newTrend << _trend;

            if (newTrend!=_trend) {
                _trend = newTrend;
                return true;
            }

            return false;
        }

        VALUE_TREND valueTrend() {
            return _trend;
        }

        VALUE_TREND calculateValueTrend() {
            if (_trendLifeTime>0) {
                if (QDateTime::currentMSecsSinceEpoch()>_lastTrendUpdate+_trendLifeTime) {
                    _trend = VALUE_TREND_NONE;
                }
            }

            return _trend;
        }

        void clear() {
            switch(_value.type()) {
            case QVariant::Int:
                _value =  0;
                break;
            case QVariant::Double:
                _value = (double)0.0;
                break;
            case QVariant::Bool:
                _value = false;
                break;
            case QVariant::String:
                _value = "";
                break;
            case QVariant::StringList:
                _value = QStringList();
                break;
            default:
                qWarning() << "Unsupported data type" << _value.typeName();
                break;
            }
        }

        void updateValue(QVariant val, bool updateTrend) {
            if (!_isFirstUpdate && updateTrend) {
                switch(_value.type()) {
                case QVariant::Int:
                case QVariant::UInt:
                case QVariant::Double:
                case QVariant::LongLong:
                case QVariant::ULongLong:
                    if (val>_value) {
                        _trend = VALUE_TREND_INCREASING;
                    } else if (val<_value) {
                        _trend = VALUE_TREND_DECREASING;
                    } else {
                        _trend = VALUE_TREND_CONSTANT;
                    }
                    _lastTrendUpdate = QDateTime::currentMSecsSinceEpoch();
                    break;
                default:
                    break;
                }
            }

            _value = val;
            _lastUpdate = QDateTime::currentMSecsSinceEpoch();

            _isFirstUpdate = false;
        }
    };

    explicit ControllerBase(VALUE_OWNER_MODE valueOwnerMode, QObject *parent = nullptr);

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

    virtual QVariant value(int index);

    void init(ControllerManager* parent, AppConfiguration* appConfig, QMqttClient *mqttClient);

    AppConfiguration* getConfig();

    void publishValue(int index);

    void publishTrend(int index);

    void publishCmd(EnumsDeclarations::MQTT_CMDS cmd);

    QList<ValueStruct> values();

    bool valueIsValid(int index);

    QVariant::Type getValueType(int index = -1);

    void setValue(int index, QVariant value, bool sendSet = false, bool ignoreCompare = false);

    QString getLabel(int index);

    void clearValue(int index);

    static QVariant parsePayload(QByteArray payload);

    //void setMode(VALUE_OWNER_MODE thisMode);

protected:
    VALUE_OWNER_MODE m_mode = VALUE_OWNER_DEFAULT;
    QList<ValueStruct> m_values;
    QStringList m_topicPath;
    QStringList m_labels;
    ControllerManager* m_parent;
    QTimer m_scheduler;

    QList<QVariant::Type> m_TypeCache;

    QString m_topicName;

    virtual bool hasSetSupport() {      // others can set values
        return false;
    }

    virtual void onInit();

    virtual void onMqttUnknownMessageReceived(QStringList topicPath, QByteArray data);

    virtual void onUnmappedMqttValueReceived(QStringList topicPath, QVariant value);

    virtual void onConnectedChanged(bool connected);

    virtual void onSetReceived(int index, QVariant value);

    virtual void onValueChanged(int index, QVariant value);

    virtual void onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd);

    virtual void onScheduleUpdate();

    void startScheduler(quint16 interval);

protected slots:

private:
    AppConfiguration* m_appConfig;
    QMqttClient *m_mqttClient;

    QMqttSubscription* m_topicValSub;
    QMqttSubscription* m_topicSetSub;
    QMqttSubscription* m_bcSub;

    QTimer m_valueUpdateTimer;
    QTimer m_valueTrendTimer;
    QTimer m_valueBCTimer;

    QMutex m_setValueMutex;

    bool _checkIndex(int index);

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

    void onCheckTrend();

    void onCheckBroadcasts();

    void onScheduleTimeout();

};

#endif // CONTROLLERBASE_H
