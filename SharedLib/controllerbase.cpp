#include "include/controllerbase.h"
#include "constants.h"
#include <QDebug>

ControllerBase::ControllerBase(QObject *parent) : QObject(parent)
{
    connect(&m_mqttClient, &QMqttClient::connected, this, &ControllerBase::_onMqttConnected);
}

AppConfiguration* ControllerBase::getConfig() {
    return m_appConfig;
}

void ControllerBase::init(AppConfiguration *appConfig) {
    qDebug() << Q_FUNC_INFO;

    m_labels = getLabelList();

    m_appConfig = appConfig;

    m_mqttClient.setHostname(m_appConfig->getString(AppConfiguration::MQTT_HOST, "localhost"));
    m_mqttClient.setPort(m_appConfig->getInt(AppConfiguration::MQTT_PORT, 1883));

    qDebug() << "Connecting to" << m_mqttClient.hostname();

    m_mqttClient.connectToHost();

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

void ControllerBase::_onMqttConnected() {
    qDebug() << Q_FUNC_INFO;

    m_topicPath = getTopicPath();
    m_topicName = m_topicPath.last();

    QStringList fullPath = buildPath(m_topicPath, MQTT_MODE_VAL, true);

    QMqttTopicFilter filter = QMqttTopicFilter(fullPath.join(MQTT_PATH_SEP));
    if (filter.isValid()) {
        qDebug() << "Subscribing to" << filter.filter();
        QMqttSubscription* sub = m_mqttClient.subscribe(filter);
        connect(sub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
    }

    // connect to bc
    QMqttSubscription* bc_sub = m_mqttClient.subscribe(QMqttTopicFilter(MQTT_PATH_BC));
    connect(bc_sub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);

    onMqttConnected();
}

QStringList ControllerBase::buildPath(QStringList paths, MQTT_MODE mode, bool addWildcard) {
    if (paths.first()!=MQTT_BASE_PATH) paths.prepend(MQTT_BASE_PATH);
    switch(mode) {
    case MQTT_MODE_VAL:
        paths.append(MQTT_VAL);
        break;
    case MQTT_MODE_SET:
        paths.append(MQTT_SET);
        break;
    }

    if (addWildcard) paths.append(MQTT_WILDCARD);
    return paths;
}

QStringList ControllerBase::cleanPath(QStringList paths) {
    if (paths.first()==MQTT_BASE_PATH) paths.removeFirst();
    return paths;
}


void ControllerBase::onInit() {
    qDebug() << Q_FUNC_INFO;
}

void ControllerBase::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;
}

void ControllerBase::publish(int index) {
    m_mqttClient.publish(QMqttTopicName(buildPath(m_topicPath << QString::number(index)).join(MQTT_PATH_SEP)), getPayload(m_values.at(index)));
}


void ControllerBase::_onMqttMessageReceived(QMqttMessage msg) {
    qDebug() << Q_FUNC_INFO << msg.topic() << msg.payload();

    QStringList topicPath = cleanPath(msg.topic().levels());
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

QByteArray ControllerBase::getPayload(QVariant value) {
    QByteArray returnData;

    if (value.type()==QVariant::Double) {
        returnData.append(MQTT_ID_DOUBLE);
        returnData.append(QByteArray::number(value.toDouble()));
    } else if (value.type()==QVariant::Int) {
        returnData.append(MQTT_ID_INTEGER);
        returnData.append(QByteArray::number(value.toInt()));
    } else if (value.type()==QVariant::String) {
        returnData.append(MQTT_ID_STRING);
        returnData.append(value.toString());
    } else if (value.type()==QVariant::Bool) {
        returnData.append(MQTT_ID_BOOL);
        returnData.append(value.toBool());
    } else {
        qWarning() << "Cannot serialize" << value.typeName();
    }

    return returnData;
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

