#include "include/controller/controllermanager.h"
#include "include/constants_qt.h"

#include "utils/databasemanager.h"

#include <QDebug>
#include <QTimer>

ControllerManager::ControllerManager(QString deviceId, QObject *parent) : QObject(parent), m_deviceId(deviceId)
{
    qDebug() << Q_FUNC_INFO << m_deviceId;
    Q_ASSERT(!m_deviceId.isEmpty());

    connect(&m_mqttClient, &QMqttClient::connected, this, &ControllerManager::_onMqttConnected);
    connect(&m_mqttClient, &QMqttClient::disconnected, this, &ControllerManager::_onMqttDisconnected);
}

void ControllerManager::init(AppConfiguration *appConfig) {
    qDebug() << Q_FUNC_INFO;

    m_appConfig = appConfig;

    m_mqttClient.setHostname(m_appConfig->getString(AppConfiguration::MQTT_HOST, "localhost"));
    m_mqttClient.setPort(static_cast<quint16>(m_appConfig->getInt(AppConfiguration::MQTT_PORT, 1883)));

    connect(&m_mqttClient, &QMqttClient::errorChanged, this, &ControllerManager::_onMqttError);
    connect(&m_mqttClient, &QMqttClient::stateChanged, this, &ControllerManager::_onMqttStateChanged);

    qDebug() << "Connecting to" << m_mqttClient.hostname();

    for (ControllerBase* c : m_controllers) {
        qDebug() << "Init" << c->getName();
        c->init(this, appConfig, &m_mqttClient);
    }

    m_mqttClient.connectToHost();

    DatabaseManager::instance()->init(appConfig);
}

void ControllerManager::registerController(ControllerBase *controller) {
    qDebug() << Q_FUNC_INFO << controller->getName();
    controller->setParent(this);
    m_controllers.insert(controller->getName(), controller);
}

void ControllerManager::_onMqttStateChanged(QMqttClient::ClientState state) {
    qDebug() << Q_FUNC_INFO << state;
}

void ControllerManager::_onMqttError(QMqttClient::ClientError error) {
    qWarning() << Q_FUNC_INFO << error;
}

ControllerBase* ControllerManager::getController(QString name) {
    if (m_controllers.contains(name)) {
        return m_controllers.value(name);
    } else {
        qFatal("Controller not found");
        return nullptr;
    }
}

void ControllerManager::_onMqttConnected() {
    qDebug() << Q_FUNC_INFO;

    m_cmdSub = m_mqttClient.subscribe(QMqttTopicFilter(buildPath(QStringList() << MQTT_PATH_CMD).join(MQTT_PATH_SEP)));
    connect(m_cmdSub, &QMqttSubscription::messageReceived, this, &ControllerManager::_onMqttCmdReceived);

    if (isServer()) {
        qDebug() << "Sending broadcast all";
        publishBC(ControllerManager::MQTT_BC_ALL);
    }

    Q_EMIT(mqttConnected());
}

void ControllerManager::_onMqttDisconnected() {
    qDebug() << Q_FUNC_INFO;

    Q_EMIT(mqttDisconnected());

    m_cmdSub->unsubscribe();
    m_cmdSub->disconnect();

    // reconnect
    QTimer::singleShot(1000, [=]() {
        qDebug() << "Reconnecting...";
        m_mqttClient.disconnectFromHost();
        m_mqttClient.connectToHost();
    } );
}

void ControllerManager::_onMqttCmdReceived(QMqttMessage msg) {
    qDebug() << Q_FUNC_INFO << msg.topic() << msg.payload();

    EnumsDeclarations::MQTT_CMDS cmd = static_cast<EnumsDeclarations::MQTT_CMDS>(ControllerBase::parsePayload(msg.payload()).toInt());
    Q_EMIT(mqttCmdReceived(cmd));
}

QString ControllerManager::getBroadcastValue(MQTT_BROADCAST_TYPE type) {
    switch(type) {
    case ControllerManager::MQTT_BC_ALL:
        return MQTT_BC_CMD_BC_ALL;
    case ControllerManager::MQTT_BC_TEMPS:
        return MQTT_PATH_TEMPS;
    case ControllerManager::MQTT_BC_RELAYS:
        return MQTT_PATH_RELAYS;
    }
    return "";
}

bool ControllerManager::isServer() {
    return m_deviceId==DEV_ID_SERVER;
}

QString ControllerManager::deviceId() {
    return m_deviceId;
}

void ControllerManager::publishBC(MQTT_BROADCAST_TYPE type) {
    publish(buildPath(QStringList() << MQTT_PATH_BC),  getBroadcastValue(type));
}

void ControllerManager::publishCmd(EnumsDeclarations::MQTT_CMDS cmd) {
    publish(buildPath(QStringList() << MQTT_PATH_CMD), static_cast<int>(cmd));
}

void ControllerManager::publish(QStringList path, QVariant value) {
    qDebug() << Q_FUNC_INFO << path << value;

    if (m_mqttClient.state()==QMqttClient::Connected) {
        m_mqttClient.publish(QMqttTopicName(path.join(MQTT_PATH_SEP)), getPayload(value));
    } else {
        qWarning() << "Cannot publish" << m_mqttClient.state();
    }
}

QStringList ControllerManager::buildPath(QStringList paths, MQTT_MODE mode, int index, bool addWildcard) {
    //qDebug() << Q_FUNC_INFO << paths;

    if (paths.first()!=MQTT_BASE_PATH) paths.prepend(MQTT_BASE_PATH);
    switch(mode) {
    case MQTT_MODE_VAL:
        paths.append(MQTT_VAL);
        break;
    case MQTT_MODE_SET:
        paths.append(MQTT_SET);
        break;
    case MQTT_MODE_TRE:
        paths.append(MQTT_TRE);
        break;
    default:
        // nothing
        break;
    }

    if (addWildcard) {
        paths.append(MQTT_WILDCARD);
    } else if (index>-1){
        paths.append(QString::number(index));
    }
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
        returnData.append(value.toBool()?"1":"0");
    } else {
        qWarning() << "Cannot serialize" << value.typeName();
    }

    //qDebug() << "Payload" << returnData;

    return returnData;
}
