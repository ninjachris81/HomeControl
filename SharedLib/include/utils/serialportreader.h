#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QByteArray>
#include <QSerialPort>
#include <QTimer>

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_SERIAL_PORT_READER)


class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortReader(QObject *parent = nullptr);

    void begin(QString portName, QSerialPort::BaudRate baudRate, quint16 readTimeoutMs, QSerialPort::Parity parity = QSerialPort::Parity::NoParity, QSerialPort::StopBits stopBits = QSerialPort::StopBits::OneStop, QSerialPort::DataBits databits = QSerialPort::DataBits::Data8);

protected slots:
    void start();

signals:
    void lineReceived(QByteArray data);

private:
    void startRestart();

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort m_serialPort;
    QByteArray m_readData;
    QTimer m_timer;
    bool m_isRestarting;

};

#endif // SERIALPORTREADER_H
