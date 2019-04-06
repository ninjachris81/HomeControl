#include "include/controllermanager.h"
#include "constants.h"
#include <QDebug>
#include <QTimer>

ControllerManager::ControllerManager(QObject *parent) : QObject(parent)
{
    connect(&m_mqttClient, &QMqttClient::connected, this, &ControllerManager::_onMqttConnected);
    connect(&m_mqttClient, &QMqttClient::disconnected, this, &ControllerManager::_onMqttDisconnected);
}

void ControllerManager::init(AppConfiguration *appConfig) {
    m_appConfig = appConfig;

    m_mqttClient.setHostname(m_appConfig->getString(AppConfiguration::MQTT_HOST, "localhost"));
    m_mqttClient.setPort(m_appConfig->getInt(AppConfiguration::MQTT_PORT, 1883));

    connect(&m_mqttClient, &QMqttClient::errorChanged, this, &ControllerManager::_onMqttError);
    connect(&m_mqttClient, &QMqttClient::stateChanged, this, &ControllerManager::_onMqttStateChanged);

    qDebug() << "Connecting to" << m_mqttClient.hostname();

    for (ControllerBase* c : m_controllers) {
        qDebug() << "Init" << c->getName();
        c->init(this, appConfig, &m_mqttClient);
    }

    m_mqttClient.connectToHost();
}

void ControllerManager::registerController(ControllerBase *controller) {
    m_controllers.insert(controller->getName(), controller);
}

void ControllerManager::_onMqttStateChanged(QMqttClient::ClientState state) {
    qDebug() << Q_FUNC_INFO << state;
}

void ControllerManager::_onMqttError(QMqttClient::ClientError error) {
    qWarning() << Q_FUNC_INFO << error;
}

ControllerBase* ControllerManager::getController(QString name) {
    return m_controllers.value(name);
}

void ControllerManager::_onMqttConnected() {
    qDebug() << Q_FUNC_INFO;

    Q_EMIT(mqttConnected());
}

void ControllerManager::_onMqttDisconnected() {
    qDebug() << Q_FUNC_INFO;

    Q_EMIT(mqttDisconnected());

    // reconnect
    QTimer::singleShot(1000, [=]() {
        qDebug() << "Reconnecting...";
        m_mqttClient.disconnectFromHost();
        m_mqttClient.connectToHost();
    } );
}

QString ControllerManager::getBroadcastValue(MQTT_BROADCAST_TYPE type) {
    switch(type) {
    case ControllerManager::MQTT_BC_ALL:
        return MQTT_BC_CMD_BC_ALL;
    case ControllerManager::MQTT_BC_TEMPS:
        return MQTT_PATH_TEMPERATURES;
    case ControllerManager::MQTT_BC_RELAYS:
        return MQTT_PATH_RELAYS;
    }
    return "";
}

void ControllerManager::publishBC(MQTT_BROADCAST_TYPE type) {
    publish(buildPath(QStringList() << MQTT_PATH_BC), getBroadcastValue(type));
}

void ControllerManager::publish(QStringList path, QVariant value) {
    if (m_mqttClient.state()==QMqttClient::Connected) {
        m_mqttClient.publish(QMqttTopicName(path.join(MQTT_PATH_SEP)), getPayload(value));
    } else {
        qWarning() << "Cannot publish" << m_mqttClient.state();
    }
}

QStringList ControllerManager::buildPath(QStringList paths, MQTT_MODE mode, bool addWildcard) {
    if (paths.first()!=MQTT_BASE_PATH) paths.prepend(MQTT_BASE_PATH);
    switch(mode) {
    case MQTT_MODE_VAL:
        paths.append(MQTT_VAL);
        break;
    case MQTT_MODE_SET:
        paths.append(MQTT_SET);
        break;
    default:
        // nothing
        break;
    }

    if (addWildcard) paths.append(MQTT_WILDCARD);
    return paths;
}

QStringList ControllerManager::cleanPath(QStringList paths) {
    if (paths.first()==MQTT_BASE_PATH) paths.removeFirst();
    return paths;
}


QByteArray ControllerManager::getPayload(QVariant value) {
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
