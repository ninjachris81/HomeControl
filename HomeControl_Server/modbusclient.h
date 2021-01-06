#ifndef MODBUSCLIENT_H
#define MODBUSCLIENT_H

#include <QObject>
#include <QModbusTcpClient>
#include <QLoggingCategory>
#include "utils/appconfiguration.h"

Q_DECLARE_LOGGING_CATEGORY(LG_MODBUS_CLIENT)

class ModbusClient : public QObject
{
    Q_OBJECT
public:
    explicit ModbusClient(AppConfiguration &appConfig, QObject *parent = nullptr);
    ~ModbusClient();

    QModbusReply *sendRawRequest(const QModbusRequest &request, int serverAddress);
    QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress);

    QString lastError();

protected:
    QModbusTcpClient m_client;

signals:
    void clientConnected();

private slots:
    void onErrorOccurred(QModbusDevice::Error error);
    void onStateOccurred(QModbusDevice::State state);

public slots:
};

#endif // MODBUSCLIENT_H
