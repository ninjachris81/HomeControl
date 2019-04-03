#include "databridge.h"
#include <QDebug>

DataBridge::DataBridge(QObject *parent) : QObject(parent)
{
    mqttClient.setHostname("rpi-adsb.fritz.box");
    mqttClient.setPort(1883);
    mqttClient.connectToHost();

    connect(&mqttClient, &QMqttClient::connected, this, &DataBridge::onConnected);
}

void DataBridge::onConnected() {
    qDebug() << Q_FUNC_INFO << mqttClient.state();

    sub = mqttClient.subscribe(QMqttTopicFilter ("test/feeds/photocell"));
    connect(sub, &QMqttSubscription::messageReceived, this, &DataBridge::onMessageReceived);
}

void DataBridge::onMessageReceived(const QMqttMessage &msg) {
    qDebug() << Q_FUNC_INFO;

    qDebug() << msg.payload();
}
