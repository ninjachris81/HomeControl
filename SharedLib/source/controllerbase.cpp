#include "include/controllerbase.h"
#include "include/constants_qt.h"
#include "include/controllermanager.h"
#include <QDebug>

ControllerBase::ControllerBase(QObject *parent) : QObject(parent), m_topicValSub(nullptr), m_topicSetSub(nullptr), m_bcSub(nullptr)
{
}

AppConfiguration* ControllerBase::getConfig() {
    return m_appConfig;
}

void ControllerBase::init(ControllerManager* parent, AppConfiguration *appConfig, QMqttClient *mqttClient) {
    qDebug() << Q_FUNC_INFO;

    m_topicPath = getTopicPath();
    m_topicName = m_topicPath.last();

    m_parent = parent;
    m_appConfig = appConfig;
    m_mqttClient = mqttClient;

    m_labels = getLabelList();
    for(int i=0;i<m_labels.count();i++) m_values.append(QVariant(getValueType(i)));
    qDebug() << Q_FUNC_INFO << m_values;

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

QVariant ControllerBase::value(int index) {
    if (index<m_values.count()) {
        return m_values.at(index);
    } else {
        qWarning() << "Invalid index" << m_values.count() << index;
        return QVariant();
    }
}

QList<QVariant> ControllerBase::values() {
    return m_values;
}

void ControllerBase::setValue(int index, QVariant value, bool sendSet) {
    qDebug() << Q_FUNC_INFO << index << value;

    if (index<m_values.count()) {
        //qDebug() << m_values.at(index) << value;
        if (m_values.at(index).cmp(value)) {
            //qDebug() << "Same value - ignoring set";
            return;
        }

        if (sendSet) {
            qDebug() << "Publish value" << index << value;
            m_parent->publish(ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_SET, index), value);
        } else {
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
    } else {
        qWarning() << "Invalid index" << m_values.count() << index;
    }
}

void ControllerBase::onMqttConnected() {
    qDebug() << Q_FUNC_INFO;

    QStringList fullPath = ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_VAL, -1, true);

    QMqttTopicFilter filter = QMqttTopicFilter(fullPath.join(MQTT_PATH_SEP));
    if (filter.isValid()) {
        qDebug() << "Subscribing to" << filter.filter();
        m_topicValSub = m_mqttClient->subscribe(filter);
        connect(m_topicValSub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
    }

    // connect to bc
    QStringList bcPath = ControllerManager::buildPath(QStringList() << MQTT_PATH_BC);
    m_bcSub = m_mqttClient->subscribe(QMqttTopicFilter(bcPath.join(MQTT_PATH_SEP)));
    connect(m_bcSub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);

    if (hasSetSupport()) {
        qDebug() << Q_FUNC_INFO << "Adding set support";
        QStringList fullPath = ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_SET, -1, true);

        QMqttTopicFilter filter = QMqttTopicFilter(fullPath.join(MQTT_PATH_SEP));
        if (filter.isValid()) {
            qDebug() << "Subscribing to" << filter.filter();
            m_topicSetSub = m_mqttClient->subscribe(filter);
            connect(m_topicSetSub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
        }
    }

    onConnectedChanged(true);
}

void ControllerBase::onMqttDisconnected() {
    qDebug() << Q_FUNC_INFO;

    if (m_topicValSub!=nullptr) {
        m_topicValSub->unsubscribe();
        m_topicValSub->disconnect();
    }

    if (m_topicSetSub!=nullptr) {
        m_topicSetSub->unsubscribe();
        m_topicSetSub->disconnect();
    }

    if (m_bcSub!=nullptr) {
        m_bcSub->unsubscribe();
        m_bcSub->disconnect();
    }

    onConnectedChanged(false);
}

void ControllerBase::onInit() {
    qDebug() << Q_FUNC_INFO;
}

void ControllerBase::publish(int index) {
    qDebug() << Q_FUNC_INFO << index << m_values.at(index);

    if (index<m_values.count()) {
        m_parent->publish(ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_VAL, index), m_values.at(index));
    } else {
        qWarning() << "Invalid index" << m_values.count() << index;
    }
}

void ControllerBase::_onMqttMessageReceived(QMqttMessage msg) {
    qDebug() << Q_FUNC_INFO << msg.topic() << msg.payload();

    QStringList topicPath = ControllerManager::cleanPath(msg.topic().levels());

    QString topicName = topicPath.at(topicPath.count()-3);
    QString valSetMode = topicPath.at(topicPath.count()-2);
    int index = topicPath.last().toInt();

    if (topicPath.first()==MQTT_PATH_BC) {
        if (topicName==MQTT_BC_CMD_BC_ALL || topicName==m_topicName) {
            broadcastValues();
        }
    } else {
        QVariant value = parsePayload(msg.payload());

        if (valSetMode==MQTT_SET && hasSetSupport()) {
            onSetReceived(index, value);
        } else if (valSetMode==MQTT_SET && !hasSetSupport()) {
            qWarning() << "Set request received, but no support";
        } else if (valSetMode==MQTT_VAL) {
            if (m_values.count()>index) {
                setValue(index, value);
            } else {
                onUnmappedMqttValueReceived(topicPath, value);
            }
        } else {
            qWarning() << "Invalid mode" << valSetMode;
        }
    }
}

void ControllerBase::broadcastValues() {
    for (int i=0;i<m_values.count();i++) {
        publish(i);
    }
}

QVariant ControllerBase::parsePayload(QByteArray payload) {
    if (payload.count()>=MQTT_MIN_MSG_SIZE) {
        switch(payload.at(0)) {
        case MQTT_ID_DOUBLE:
            return QByteArray(payload.mid(1)).toDouble();
        case MQTT_ID_INTEGER:
            return QByteArray(payload.mid(1)).toInt();
        case MQTT_ID_STRING:
            return QString(QByteArray((payload.mid(1))));
        case MQTT_ID_BOOL:
            return QByteArray(payload.mid(1)).toInt()==1;
        }
    }

    return QVariant();
}

void ControllerBase::onMqttUnknownMessageReceived(QStringList topicPath, QByteArray data) {
    qWarning() << Q_FUNC_INFO << topicPath << data;
}

void ControllerBase::onUnmappedMqttValueReceived(QStringList topicPath, QVariant value) {
    qWarning() << Q_FUNC_INFO << topicPath << value;
}

void ControllerBase::onValueChanged(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;
}

void ControllerBase::onConnectedChanged(bool connected) {
    qDebug() << Q_FUNC_INFO << connected;
}

void ControllerBase::onSetReceived(int index, QVariant value) {
    qDebug() << Q_FUNC_INFO << index << value;

    setValue(index, value);
    publish(index);
}
