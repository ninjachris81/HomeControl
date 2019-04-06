#include "include/controllerbase.h"
#include "constants.h"
#include "include/controllermanager.h"
#include <QDebug>

ControllerBase::ControllerBase(QObject *parent) : QObject(parent), m_topicSub(nullptr), m_bcSub(nullptr)
{
}

AppConfiguration* ControllerBase::getConfig() {
    return m_appConfig;
}

void ControllerBase::init(ControllerManager* parent, AppConfiguration *appConfig, QMqttClient *mqttClient) {
    qDebug() << Q_FUNC_INFO;

    m_parent = parent;
    m_appConfig = appConfig;
    m_mqttClient = mqttClient;

    m_labels = getLabelList();

    connect(parent, &ControllerManager::mqttConnected, this, &ControllerBase::onMqttConnected);
    connect(parent, &ControllerManager::mqttDisconnected, this, &ControllerBase::onMqttDisconnected);

    onInit();
}

QString ControllerBase::getLabel(int index) {
    if (index<m_labels.count()) {
        return m_labels.at(index);
    } else {
        return "Item " + QString::number(index);
    }
}

void ControllerBase::clearValue(int index) {
    switch(getValueType(index)) {
    case QVariant::Int:
        m_values[index] = 0;
        break;
    case QVariant::Double:
        m_values[index] = (double)0.0;
        break;
    case QVariant::Bool:
        m_values[index] = false;
        break;
    case QVariant::String:
        m_values[index] = "";
        break;
    case QVariant::StringList:
        m_values[index] = QStringList();
        break;
    }
}

QList<QVariant> ControllerBase::getValues() {
    return m_values;
}

void ControllerBase::setValue(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    switch(getValueType(index)) {
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::Bool:
    case QVariant::String:
        m_values[index] = value;
        break;
    case QVariant::StringList:
        QStringList list = m_values[index].toStringList();
        list.append(value.toString());
        m_values[index] = list;
        break;
    }

    onValueChanged(index, m_values[index]);
    Q_EMIT(valueChanged(index, m_values[index]));
}

void ControllerBase::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;

    m_topicPath = getTopicPath();
    m_topicName = m_topicPath.last();

    QStringList fullPath = ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_VAL, true);

    QMqttTopicFilter filter = QMqttTopicFilter(fullPath.join(MQTT_PATH_SEP));
    if (filter.isValid()) {
        qDebug() << "Subscribing to" << filter.filter();
        m_topicSub = m_mqttClient->subscribe(filter);
        connect(m_topicSub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
    }

    // connect to bc
    m_bcSub = m_mqttClient->subscribe(QMqttTopicFilter(MQTT_PATH_BC));
    connect(m_bcSub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
}

void ControllerBase::onMqttDisconnected() {
    qDebug() << Q_FUNC_INFO;

    if (m_topicSub!=nullptr) m_topicSub->unsubscribe();
    if (m_bcSub!=nullptr) m_bcSub->unsubscribe();
}

void ControllerBase::onInit() {
    qDebug() << Q_FUNC_INFO;
}

void ControllerBase::publish(int index) {
    m_parent->publish(ControllerManager::buildPath(m_topicPath << QString::number(index)), m_values.at(index));
}

void ControllerBase::_onMqttMessageReceived(QMqttMessage msg) {
    qDebug() << Q_FUNC_INFO << msg.topic() << msg.payload();

    QStringList topicPath = ControllerManager::cleanPath(msg.topic().levels());
    QString topicName = topicPath.at(topicPath.count()-2);
    int index = topicPath.last().toInt();

    if (topicPath.first()==MQTT_PATH_BC) {
        if (topicName==MQTT_BC_CMD_BC_ALL || topicName==m_topicName) {
            for (int i=0;i<m_values.count();i++) {
                publish(i);
            }
        }
    } else {
        if (msg.payload().count()>=MQTT_MIN_MSG_SIZE) {
            switch(msg.payload().at(0)) {
            case MQTT_ID_DOUBLE: {
                double d = QByteArray(msg.payload().mid(1)).toDouble();
                if (m_values.count()>index) {
                    setValue(index, d);
                } else {
                    onUnmappedMqttDoubleReceived(topicPath, d);
                }
                break;
            }
            case MQTT_ID_INTEGER: {
                int i = QByteArray(msg.payload().mid(1)).toInt();
                if (m_values.count()>index) {
                    setValue(index, i);
                } else {
                    onUnmappedMqttDoubleReceived(topicPath, i);
                }
                break;
            }
            case MQTT_ID_STRING: {
                QString s = QString(QByteArray((msg.payload().mid(1))));
                if (m_values.count()>index) {
                    setValue(index, s);
                } else {
                    onUnmappedMqttStringReceived(topicPath, s);
                }
                break;
            }
            case MQTT_ID_BOOL: {
                bool b = QByteArray(msg.payload().mid(1)).toInt()==1;
                if (m_values.count()>index) {
                    setValue(index, b);
                } else {
                    onUnmappedMqttBoolReceived(topicPath, b);
                }
                break;
            }
            default:
                onMqttUnknownMessageReceived(topicPath, msg.payload());
            }
        } else {
            onMqttUnknownMessageReceived(topicPath, msg.payload());
        }
    }
}


void ControllerBase::onMqttUnknownMessageReceived(QStringList topicPath, QByteArray data) {
    qWarning() << Q_FUNC_INFO << topicPath << data;
}

void ControllerBase::onUnmappedMqttDoubleReceived(QStringList topicPath, double value) {
    qWarning() << Q_FUNC_INFO << topicPath << value;
}

void ControllerBase::onUnmappedMqttIntReceived(QStringList topicPath, int value) {
    qWarning() << Q_FUNC_INFO << topicPath << value;
}

void ControllerBase::onUnmappedMqttStringReceived(QStringList topicPath, QString value) {
    qWarning() << Q_FUNC_INFO << topicPath << value;
}

void ControllerBase::onUnmappedMqttBoolReceived(QStringList topicPath, bool value) {
    qWarning() << Q_FUNC_INFO << topicPath << value;
}

void ControllerBase::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}

