#include "include/controller/controllerbase.h"
#include "include/controller/controllermanager.h"
#include "include/constants_qt.h"
#include <QDebug>
#include <QMetaEnum>


ControllerBase::ControllerBase(VALUE_OWNER_MODE valueOwnerMode, QObject *parent) : QObject(parent), m_mode(valueOwnerMode), m_topicValSub(nullptr), m_topicSetSub(nullptr), m_bcSub(nullptr)
{
    connect(&m_valueUpdateTimer, &QTimer::timeout, this, &ControllerBase::onCheckValue);
    m_valueUpdateTimer.setInterval(1000);
    m_valueUpdateTimer.start();
}

AppConfiguration* ControllerBase::getConfig() {
    return m_appConfig;
}

bool ControllerBase::isValueOwner(int index) {
    Q_UNUSED(index);

    switch(m_mode) {
    case VALUE_OWNER_SERVER: return m_parent->isServer();
    case VALUE_OWNER_CLIENT: return !m_parent->isServer();
    case VALUE_OWNER_EXTERNAL_SENSOR: return false;
    }

    return false;
}

ControllerBase::VALUE_BC_INTERVAL ControllerBase::getValueBCInterval(int index) {
    Q_UNUSED(index);

    return VALUE_BC_NONE;
}

qint64 ControllerBase::getValueTrendLifetime(int index) {
    Q_UNUSED(index);
    return VALUE_TT_NONE;
}

void ControllerBase::init(ControllerManager* parent, AppConfiguration *appConfig, QMqttClient *mqttClient) {
    qDebug() << Q_FUNC_INFO;

    if (!getEnumName().isEmpty()) {
        int enumId = EnumsDeclarations::staticMetaObject.indexOfEnumerator(getEnumName().toStdString().c_str());
        Q_ASSERT(enumId>=0);
        Q_ASSERT(EnumsDeclarations::staticMetaObject.enumerator(enumId).keyCount()==getLabelList().count());
    }
    if (!getValueTypes().isEmpty()) {
        int enumId = EnumsDeclarations::staticMetaObject.indexOfEnumerator(getEnumName().toStdString().c_str());
        Q_ASSERT(enumId>=0);
        Q_ASSERT(EnumsDeclarations::staticMetaObject.enumerator(enumId).keyCount()==getValueTypes().count());
    }

    m_topicPath = getTopicPath();
    m_topicName = m_topicPath.last();

    m_parent = parent;
    m_appConfig = appConfig;
    m_mqttClient = mqttClient;

    m_labels = getLabelList();
    for(int i=0;i<m_labels.count();i++) {
        ValueStruct t;
        t._lifeTime = getValueLifetime(i);
        t._value =  QVariant(getValueType(i));
        t._trendLifeTime = getValueTrendLifetime(i);
        m_values.append(t);
    }

    if (m_parent->isServer()) {
        connect(&m_valueBCTimer, &QTimer::timeout, this, &ControllerBase::onCheckBroadcasts);
        m_valueBCTimer.setInterval(VALUE_BC_FASTEST);
        m_valueBCTimer.start();
    }

    if (m_parent->isServer()) {
        connect(&m_valueTrendTimer, &QTimer::timeout, this, &ControllerBase::onCheckTrend);
        m_valueTrendTimer.setInterval(5000);
        m_valueTrendTimer.start();
    }

    connect(parent, &ControllerManager::mqttConnected, this, &ControllerBase::onMqttConnected);
    connect(parent, &ControllerManager::mqttDisconnected, this, &ControllerBase::onMqttDisconnected);
    connect(parent, &ControllerManager::mqttCmdReceived, this, &ControllerBase::_onCmdReceived);

    onInit();
}

QList<QVariant::Type> ControllerBase::getValueTypes() {
    return QList<QVariant::Type>();
}

QVariant::Type ControllerBase::getDefaultValueType() {
    return QVariant::String;
}

QVariant::Type ControllerBase::getValueType(int index) {
    if (index==-1) {
        return getDefaultValueType();
    } else {
        if (m_TypeCache.isEmpty()) {
            m_TypeCache = getValueTypes();
        }

        if (m_TypeCache.isEmpty()) {
            return getDefaultValueType();
        } else if (index < m_TypeCache.size()) {
            return m_TypeCache.at(index);
        } else {
            qWarning() << "Invalid value type index" << index;
            return QVariant::Invalid;
        }
    }
}

QString ControllerBase::getLabel(int index) {
    if (_checkIndex(index)) {
        return m_labels.at(index);
    } else {
        return "Item " + QString::number(index);
    }
}

void ControllerBase::clearValue(int index) {
    m_values[index].clear();
}

QVariant ControllerBase::value(int index) {
    if (_checkIndex(index)) {
        return m_values[index]._value;
    } else {
        qWarning() << Q_FUNC_INFO << "Invalid index" << m_values.count() << index;
        return QVariant();
    }
}

bool ControllerBase::valueIsValid(int index) {
    if (_checkIndex(index)) {
        return m_values[index].isValid();
    } else {
        qWarning() << Q_FUNC_INFO << "Invalid index" << m_values.count() << index;
        return false;
    }
}

QList<ControllerBase::ValueStruct> ControllerBase::values() {
    return m_values;
}

void ControllerBase::setValue(int index, QVariant value, bool sendSet, bool ignoreCompare) {
    qDebug() << Q_FUNC_INFO << index << value << sendSet << ignoreCompare;

    if (_checkIndex(index)) {
        //qDebug() << m_values.at(index) << value;
        if (!ignoreCompare) {
            if (m_values[index].compareTo(value)) {
                //qDebug() << "Same value - ignoring set";
                m_values[index].updateValue(value, m_parent->isServer());
                return;
            }
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
                    m_values[index].updateValue(value, m_parent->isServer());
                    break;
                case QVariant::StringList: {
                    QStringList list = m_values[index]._value.toStringList();
                    list.append(value.toString());
                    m_values[index].updateValue(list, m_parent->isServer());
                    break;
                }
                default:
                    qWarning() << "Unsupported data type" << getValueType(index);
                    break;
            }

            onValueChanged(index, m_values[index]._value);
            Q_EMIT(valueChanged(index, m_values[index]._value));
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

    if (!m_parent->isServer()) {
        QStringList fullPath = ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_TRE, -1, true);

        QMqttTopicFilter filter = QMqttTopicFilter(fullPath.join(MQTT_PATH_SEP));
        if (filter.isValid()) {
            qDebug() << "Subscribing to" << filter.filter();
            m_topicValSub = m_mqttClient->subscribe(filter);
            connect(m_topicValSub, &QMqttSubscription::messageReceived, this, &ControllerBase::_onMqttMessageReceived);
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

void ControllerBase::publishValue(int index) {
    qDebug() << Q_FUNC_INFO << index << m_values.at(index)._value;

    if (_checkIndex(index)) {
        m_parent->publish(ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_VAL, index), m_values[index]._value);
    } else {
        qWarning() << "Invalid index" << m_values.count() << index;
    }
}

void ControllerBase::publishTrend(int index) {
    qDebug() << Q_FUNC_INFO << index << m_values[index].calculateValueTrend();

    if (_checkIndex(index)) {
        m_parent->publish(ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_TRE, index), m_values[index].calculateValueTrend());
    } else {
        qWarning() << "Invalid index" << m_values.count() << index;
    }
}

void ControllerBase::publishCmd(EnumsDeclarations::MQTT_CMDS cmd) {
    qDebug() << Q_FUNC_INFO << cmd;
    m_parent->publishCmd(cmd);
}

void ControllerBase::_onMqttMessageReceived(QMqttMessage msg) {
    qDebug() << Q_FUNC_INFO << msg.topic() << msg.payload();

    QStringList topicPath = ControllerManager::cleanPath(msg.topic().levels());
    QVariant value = parsePayload(msg.payload());

    if (topicPath.first()==MQTT_PATH_BC) {
        if (value==MQTT_BC_CMD_BC_ALL || value==m_topicName) {
            broadcastValues();
        }
    } else {
        QString topicName = topicPath.at(topicPath.count()-3);
        QString valSetMode = topicPath.at(topicPath.count()-2);
        int index = topicPath.last().toInt();

        if (valSetMode==MQTT_SET && hasSetSupport()) {
            onSetReceived(index, value);
        } else if (valSetMode==MQTT_SET && !hasSetSupport()) {
            qWarning() << "Set request received, but no support";
        } else if (valSetMode==MQTT_VAL) {
            if (_checkIndex(index)) {
                setValue(index, value);
            } else {
                onUnmappedMqttValueReceived(topicPath, value);
            }
        } else if (valSetMode==MQTT_TRE) {
            if (!m_parent->isServer()) {
                if (_checkIndex(index)) {
                    if (m_values[index].valueTrendReceived(value.toInt())) {
                        qDebug() << Q_FUNC_INFO << "Trend changed" << index << m_values[index]._trend;
                        Q_EMIT(valueTrendChanged(index));
                    }
                } else {
                    qWarning() << "Invalid index for trend" << index;
                }
            }
        } else {
            qWarning() << "Invalid mode" << valSetMode;
        }
    }
}

void ControllerBase::broadcastValues() {
    for (int i=0;i<m_values.count();i++) {
        if (isValueOwner(i)) publishValue(i);
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

/*
void ControllerBase::setMode(ControllerBase::VALUE_OWNER_MODE thisMode) {
    m_mode = thisMode;
}*/

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
    publishValue(index);
}

void ControllerBase::onCheckValue() {
    for (uint8_t i=0;i<m_values.count();i++) {
        bool oldValid = m_values[i]._wasValid;

        if (oldValid!=m_values[i].isValid()) {
            Q_EMIT(valueValidChanged(i));

            // invalidate trend as well
            if (!m_values[i].isValid()) {
                m_values[i]._trend = VALUE_TREND_NONE;
                Q_EMIT(valueTrendChanged(i));
            }
        }
    }
}

void ControllerBase::onCheckTrend() {
    qDebug() << Q_FUNC_INFO;

    for (uint8_t i=0;i<m_values.count();i++) {
        if (m_values[i].isValid() && getValueTrendLifetime(i)>VALUE_TT_NONE) {
            publishTrend(i);
        }
    }
}


void ControllerBase::onCheckBroadcasts() {
    for (uint8_t i=0;i<m_values.count();i++) {
        if (isValueOwner(i) && m_values[i].isValid() && getValueBCInterval(i)!=VALUE_BC_NONE) {
            publishValue(i);
        }
    }
}

bool ControllerBase::_checkIndex(int index) {
    if (index<m_values.count()) {
        return true;
    } else {
        qWarning() << "Invalid index" << m_values.count() << index;
        return false;
    }
}

void ControllerBase::_onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    onCmdReceived(cmd);
}

void ControllerBase::onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    Q_UNUSED(cmd);

    // DO NOTHING
}
