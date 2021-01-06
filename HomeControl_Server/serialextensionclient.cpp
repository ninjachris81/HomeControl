#include "serialextensionclient.h"

Q_LOGGING_CATEGORY(LG_SERIAL_EXTENSION_CLIENT, "Serial Extension Client");

SerialExtensionClient::SerialExtensionClient(ControllerManager *controllerManager, AppConfiguration &appConfig, QObject *parent) : QObject(parent)
{
    m_eventController = static_cast<EventController*>(controllerManager->getController(EventController::CONTROLLER_NAME));
    m_currentController = static_cast<CurrentController*>(controllerManager->getController(CurrentController::CONTROLLER_NAME));
    m_powerController = static_cast<PowerController*>(controllerManager->getController(PowerController::CONTROLLER_NAME));

#ifdef WIN32
    QString port = "COM15";
#else
    QString port = appConfig.getString("ARDUINO_SERIAL_EXTENSION_PORT", "/dev/ttyUSB0");
#endif

    connect(&serialPortReader, &SerialPortReader::lineReceived, this, &SerialExtensionClient::onLineReceived);
    serialPortReader.begin(port, QSerialPort::Baud115200, SERIAL_TIMEOUT_MS);
}

void SerialExtensionClient::onLineReceived(QByteArray data) {
    qDebug() << Q_FUNC_INFO << data << data.length();

    if (data.endsWith("\n")) data.chop(1);
    if (data.endsWith("\r")) data.chop(1);

    double val = data.mid(3).toDouble();

    if (data.startsWith("BS:")) {
        // bell state
        if (val != 0.0) {
            m_eventController->raiseEvent(EnumsDeclarations::EVENTS_DOOR_BELL);
        }
    } else if (data.startsWith("BC:")) {
        // base current
        m_currentController->setValue(EnumsDeclarations::CURRENTS_MAIN_BASEMENT, val);
        m_currentController->publishValue(EnumsDeclarations::CURRENTS_MAIN_BASEMENT);

        m_powerController->setValue(EnumsDeclarations::POWERS_MAIN_BASEMENT, val *  230.0);
        m_powerController->publishValue(EnumsDeclarations::POWERS_MAIN_BASEMENT);
    } else if (data.startsWith("OP")) {
        // overall power
        m_powerController->setValue(EnumsDeclarations::POWERS_MAIN, val);
        m_powerController->publishValue(EnumsDeclarations::POWERS_MAIN);
    }
}
