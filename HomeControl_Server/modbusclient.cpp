#include "modbusclient.h"
#include <QVariant>
#include <QTimer>
#include <QThread>

Q_LOGGING_CATEGORY(LG_MODBUS_CLIENT, "Modbus Client")

ModbusClient::ModbusClient(AppConfiguration &appConfig, QObject *parent) : QObject(parent)
{
    m_client.setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant::fromValue(appConfig.getString("SYSTA_SMART_C_HOST", "systasmartc.fritz.box")));
    m_client.setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant::fromValue(appConfig.getInt("SYSTA_SMART_C_PORT", 502)));

    connect(&m_client, &QModbusTcpClient::errorOccurred, this, &ModbusClient::onErrorOccurred);
    connect(&m_client, &QModbusTcpClient::stateChanged, this, &ModbusClient::onStateOccurred);

    m_client.connectDevice();
}

ModbusClient::~ModbusClient() {
    qCDebug(LG_MODBUS_CLIENT) << Q_FUNC_INFO;
    m_client.disconnectDevice();
}

void ModbusClient::onErrorOccurred(QModbusDevice::Error error) {
    qCDebug(LG_MODBUS_CLIENT) << Q_FUNC_INFO << error;
}

void ModbusClient::onStateOccurred(QModbusDevice::State state) {
    qCDebug(LG_MODBUS_CLIENT) << Q_FUNC_INFO << state;

    if (state==QModbusDevice::UnconnectedState) {
        QTimer::singleShot(1000, [this]() {
            m_client.connectDevice();
        });
        Q_EMIT(clientDisonnected());
    } else if (state==QModbusDevice::ConnectedState) {
        Q_EMIT(clientConnected());
    }
}

QModbusReply *ModbusClient::sendReadRequest(const QModbusDataUnit &read, int serverAddress) {
    return m_client.sendReadRequest(read, serverAddress);
}

QModbusReply *ModbusClient::sendRawRequest(const QModbusRequest &request, int serverAddress) {
    return m_client.sendRawRequest(request, serverAddress);
}

QString ModbusClient::lastError() {
    return m_client.errorString();
}
