#include "utils/serialportreader.h"

Q_LOGGING_CATEGORY(LG_SERIAL_PORT_READER, "SerialPortReader");

SerialPortReader::SerialPortReader(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);
}

void SerialPortReader::begin(QString portName, QSerialPort::BaudRate baudRate, quint16 readTimeoutMs, QSerialPort::Parity parity, QSerialPort::StopBits stopBits, QSerialPort::DataBits databits) {
    qCDebug(LG_SERIAL_PORT_READER) << Q_FUNC_INFO << portName << baudRate << readTimeoutMs;

    m_serialPort.setPortName(portName);
    m_serialPort.setBaudRate(baudRate);
    m_serialPort.setParity(parity);
    m_serialPort.setStopBits(stopBits);
    m_serialPort.setDataBits(databits);

    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::handleReadyRead);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::handleError);
    m_timer.setInterval(readTimeoutMs);

    start();
}

void SerialPortReader::start() {
    qCDebug(LG_SERIAL_PORT_READER) << Q_FUNC_INFO;

    m_isRestarting = false;
    m_timer.start();
    if (m_serialPort.open(QIODevice::ReadOnly)) {
        qCDebug(LG_SERIAL_PORT_READER) << "Opened port" << m_serialPort.portName();
    } else {
        startRestart();
    }
}

void SerialPortReader::handleReadyRead() {
    qCDebug(LG_SERIAL_PORT_READER) << Q_FUNC_INFO << m_serialPort.bytesAvailable();
    m_timer.stop();

    while(m_serialPort.canReadLine()) {
        emit lineReceived(m_serialPort.readLine());
    }

    m_timer.start();
}

void SerialPortReader::handleTimeout() {
    qCWarning(LG_SERIAL_PORT_READER) << Q_FUNC_INFO;
    startRestart();
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError) {
    if (m_isRestarting) return;

    if (serialPortError != QSerialPort::NoError) {
        qCWarning(LG_SERIAL_PORT_READER) << Q_FUNC_INFO << serialPortError;
        startRestart();
    }
}

void SerialPortReader::startRestart() {
    qCDebug(LG_SERIAL_PORT_READER) << Q_FUNC_INFO;
    m_isRestarting = true;

    m_serialPort.close();

    QTimer::singleShot(5000, [this]() {
        start();
    } );
}
