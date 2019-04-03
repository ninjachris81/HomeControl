#ifndef DATABRIDGE_H
#define DATABRIDGE_H

#include <QObject>
#include <QtSensors/QSensor>
#include <QMqttClient>

class DataBridge : public QObject
{
    Q_OBJECT
public:
    explicit DataBridge(QObject *parent = nullptr);

private:
    QMap<QString, QSensor*> sensors;
    QMqttClient mqttClient;
    QMqttSubscription *sub;

signals:

private slots:
    void onConnected();
    void onMessageReceived(const QMqttMessage &msg);

public slots:
};

#endif // DATABRIDGE_H
