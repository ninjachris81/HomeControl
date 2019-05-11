#include "include/controller/controllerbase.h"
#include "include/controller/controllermanager.h"
#include "include/constants_qt.h"
#include <QDebug>
#include <QMetaEnum>


ControllerBase::ControllerBase(QObject *parent) : QObject(parent), m_topicValSub(nullptr), m_topicSetSub(nullptr), m_bcSub(nullptr)
{
    connect(&m_validityTimer, &QTimer::timeout, this, &ControllerBase::onCheckValidity);
    m_validityTimer.setInterval(1000);
    m_validityTimer.start();
}

AppConfiguration* ControllerBase::getConfig() {
    return m_appConfig;
}

bool ControllerBase::isValueOwner(int index) {
    Q_UNUSED(index);

    return m_mode==VALUE_OWNER_SERVER;
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
        t.lifeTime = getValueLifetime(i);
        t.lastUpdate =0;
        t.value =  QVariant(getValueType(i));
        t.wasValid = false;
        m_values.append(t);
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
    if (index<m_labels.count()) {
        return m_labels.at(index);
    } else {
        return "Item " + QString::number(index);
    }
}

void ControllerBase::clearValue(int index) {
    switch(getValueType(index)) {
    case QVariant::Int:
        m_values[index].value = 0;
        break;
    case QVariant::Double:
        m_values[index].value = (double)0.0;
        break;
    case QVariant::Bool:
        m_values[index].value = false;
        break;
    case QVariant::String:
        m_values[index].value = "";
        break;
    case QVariant::StringList:
        m_values[index].value = QStringList();
        break;
    }
}

QVariant ControllerBase::value(int index) {
    if (index<m_values.count()) {
        return m_values.at(index).value;
    } else {
        qWarning() << Q_FUNC_INFO << "Invalid index" << m_values.count() << index;
        return QVariant();
    }
}

bool ControllerBase::valueIsValid(int index) {
    if (index<m_values.count()) {
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
    qDebug() << Q_FUNC_INFO << index << value;

    if (index<m_values.count()) {
        //qDebug() << m_values.at(index) << value;
        if (!ignoreCompare) {
            if (m_values[index].compareTo(value)) {
                //qDebug() << "Same value - ignoring set";
                m_values[index].updateValue(value);
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
                m_values[index].updateValue(value);
                break;
            case QVariant::StringList:
                QStringList list = m_values[index].value.toStringList();
                list.append(value.toString());
                m_values[index].updateValue(list);
                break;
            }

            onValueChanged(index, m_values[index].value);
            Q_EMIT(valueChanged(index, m_values[index].value));
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
    qDebug() << Q_FUNC_INFO << index << m_values.at(index).value;

    if (index<m_values.count()) {
        m_parent->publish(ControllerManager::buildPath(m_topicPath, ControllerManager::MQTT_MODE_VAL, index), m_values.at(index).value);
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
        if (isValueOwner(i)) publish(i);
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


void ControllerBase::setMode(ControllerBase::VALUE_OWNER_MODE thisMode) {
    m_mode = thisMode;
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

void ControllerBase::onCheckValidity() {
    for (uint8_t i=0;i<m_values.count();i++) {
        bool oldValid = m_values[i].wasValid;

        if (oldValid!=m_values[i].isValid()) {
            Q_EMIT(valueValidChanged(i));
        }
    }
}

void ControllerBase::_onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    onCmdReceived(cmd);
}

void ControllerBase::onCmdReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    Q_UNUSED(cmd);

    // DO NOTHING
}
