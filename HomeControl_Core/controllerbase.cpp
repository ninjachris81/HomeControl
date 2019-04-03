#include "controllerbase.h"
#include "constants.h"
#include <QDebug>

ControllerBase::ControllerBase(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{
    connect(&m_mqttClient, &QMqttClient::connected, this, &ControllerBase::_onMqttConnected);
}

AppConfiguration* ControllerBase::getConfig() {
    return m_appConfig;
}

QMqttTopicFilter ControllerBase::getTopicFilter() {
    return QMqttTopicFilter();
}

void ControllerBase::init() {
    qDebug() << Q_FUNC_INFO;

    m_mqttClient.setHostname(m_appConfig->getString(AppConfiguration::MQTT_HOST, "localhost"));
    m_mqttClient.setPort(m_appConfig->getInt(AppConfiguration::MQTT_PORT, 1883));

    qDebug() << "Connecting to" << m_mqttClient.hostname();

    m_mqttClient.connectToHost();

    onInit();
}

void ControllerBase::_onMqttConnected() {
    qDebug() << Q_FUNC_INFO;

    QMqttTopicFilter filter = getTopicFilter();
    if (filter.isValid()) {
        qDebug() << "Subscribing to" << filter.filter();
        QMqttSubscription* sub = m_mqttClient.subscribe(filter);
        connect(sub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
    }

    // connect to own
    QMqttSubscription* bc_sub = m_mqttClient.subscribe(QMqttTopicFilter(MQTT_PATH_BC_REQ));
    connect(bc_sub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);

    onMqttConnected();
}

void ControllerBase::onInit() {
    qDebug() << Q_FUNC_INFO;
}

void ControllerBase::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void ControllerBase::_onMqttMessageReceived(QMqttMessage msg) {
    QString topicName = msg.topic().name();

    if (topicName==MQTT_PATH_BC_REQ) {
        for (QString key : m_valueMap.keys()) {
            m_mqttClient.publish(QMqttTopicName(key), getPayload(key, m_valueMap.value(key)));
        }
    } else {
        if (msg.payload().count()>0) {
            switch(msg.payload().at(0)) {
            case MQTT_ID_DOUBLE: {
                double d = QByteArray(msg.payload().mid(1)).toDouble();
                if (m_valueMap.contains(topicName)) {
                    m_valueMap.insert(topicName, d);
                    onValueChanged(topicName, d);
                } else {
                    onMqttDoubleReceived(topicName, d);
                }
                break;
            }
            case MQTT_ID_INTEGER: {
                int i = QByteArray(msg.payload().mid(1)).toInt();
                if (m_valueMap.contains(topicName)) {
                    m_valueMap.insert(topicName, i);
                    onValueChanged(topicName, i);
                } else {
                    onMqttDoubleReceived(topicName, i);
                }
                break;
            }
            case MQTT_ID_STRING: {
                QString s = QString(QByteArray((msg.payload().mid(1))));
                if (m_valueMap.contains(topicName)) {
                    m_valueMap.insert(topicName, s);
                    onValueChanged(topicName, s);
                } else {
                    onMqttStringReceived(topicName, s);
                }
                break;
            }
            case MQTT_ID_BOOL: {
                bool b = QByteArray(msg.payload().mid(1)).toInt()==1;
                if (m_valueMap.contains(topicName)) {
                    m_valueMap.insert(topicName, b);
                    onValueChanged(topicName, b);
                } else {
                    onMqttBoolReceived(topicName, b);
                }
                break;
            }
            default:
                onMqttUnknownMessageReceived(topicName, msg.payload());
            }
        } else {
            onMqttUnknownMessageReceived(topicName, msg.payload());
        }
    }
}

QByteArray ControllerBase::getPayload(QString topic, QVariant value) {
    QByteArray returnData;

    if (topic.endsWith(MQTT_ID_DOUBLE)) {
        returnData.append(MQTT_ID_DOUBLE);
        returnData.append(QByteArray::number(value.toDouble()));
    } else if (topic.endsWith(MQTT_ID_INTEGER)) {
        returnData.append(MQTT_ID_INTEGER);
        returnData.append(QByteArray::number(value.toInt()));
    } else if (topic.endsWith(MQTT_ID_STRING)) {
        returnData.append(MQTT_ID_STRING);
        returnData.append(value.toString());
    } else if (topic.endsWith(MQTT_ID_BOOL)) {
        returnData.append(MQTT_ID_BOOL);
        returnData.append(value.toBool());
    } else {
        qWarning() << "Cannot serialize" << topic;
    }

    return returnData;
}

void ControllerBase::onMqttUnknownMessageReceived(QString topic, QByteArray data) {
    qDebug() << Q_FUNC_INFO << topic << data;
}

void ControllerBase::onMqttDoubleReceived(QString topic, double value) {
    qDebug() << Q_FUNC_INFO << topic << value;
}

void ControllerBase::onMqttIntReceived(QString topic, int value) {
    qDebug() << Q_FUNC_INFO << topic << value;
}

void ControllerBase::onMqttStringReceived(QString topic, QString value) {
    qDebug() << Q_FUNC_INFO << topic << value;
}

void ControllerBase::onMqttBoolReceived(QString topic, bool value) {
    qDebug() << Q_FUNC_INFO << topic << value;
}

void ControllerBase::onValueChanged(QString topic, QVariant value) {
    qDebug() << Q_FUNC_INFO << topic << value;
}

