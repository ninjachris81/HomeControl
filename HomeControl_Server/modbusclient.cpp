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

void ModbusClient::onErrorOccurred(QModbusDevice::Error error) {
    qCDebug(LG_MODBUS_CLIENT) << Q_FUNC_INFO << error;
}

void ModbusClient::onStateOccurred(QModbusDevice::State state) {
    qCDebug(LG_MODBUS_CLIENT) << Q_FUNC_INFO << state;

    if (state==QModbusDevice::UnconnectedState) {
        QTimer::singleShot(1000, [this]() {
            m_client.connectDevice();
        });
    } else if (state==QModbusDevice::ConnectedState) {
        Q_EMIT(clientConnected());
    }
}
