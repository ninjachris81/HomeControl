#include "include/controller/pvcontroller.h"
#include "include/controller/settingscontroller.h"
#include "include/controller/tempcontroller.h"
#include "include/controller/humiditycontroller.h"
#include "include/controller/currentcontroller.h"
#include "include/controller/controllermanager.h"

#include "include/constants_qt.h"
#include "include/controller/dataloggercontroller.h"

#include <QOperatingSystemVersion>
#include <QJsonDocument>
#include <QJsonObject>

QString PvController::CONTROLLER_NAME = QStringLiteral("PvController");
Q_LOGGING_CATEGORY(LG_PV_CONTROLLER, "PvController");

PvController::PvController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
{
}

QString PvController::getName() {
    return CONTROLLER_NAME;
}

QStringList PvController::getTopicPath() {
    return QStringList();
}

QStringList PvController::getLabelList() {
    return QStringList();
}

QString PvController::getEnumName() {
    return "";
}

QVariant::Type PvController::getDefaultValueType() {
    return QVariant::Invalid;
}

qint64 PvController::getValueLifetime(int index) {
    Q_UNUSED(index)
    return LIFETIME_ALWAYS_VALID;
}

qint64 PvController::getValueTrendLifetime(int index) {
    Q_UNUSED(index)
    return VALUE_TT_FAST;
}

void PvController::onInit() {
    qCDebug(LG_PV_CONTROLLER) << Q_FUNC_INFO;

    if (m_parent->deviceId()==DEV_ID_ZERO) {
        if (QOperatingSystemVersion::current().type()==QOperatingSystemVersion::OSType::Windows) {
            qCDebug(LG_PV_CONTROLLER) << "Ignoring serial commands";
        } else {
            m_SerialPortReader.begin(m_parent->appConfig()->getString("PV_SERIAL_PORT", "/dev/serial0"), static_cast<QSerialPort::BaudRate>(m_parent->appConfig()->getInt("PV_SERIAL_BAUDRATE", 115200)), 20000);
            connect(&m_SerialPortReader, &SerialPortReader::lineReceived, this, &PvController::onLineReceived);
        }
    }
}

void PvController::onLineReceived(QByteArray data) {
    qCDebug(LG_PV_CONTROLLER) << Q_FUNC_INFO << data;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (doc.isObject()) {
        if (doc.object().keys().contains("err")) {
            qCWarning(LG_PV_CONTROLLER) << "Received error" << doc.object().value("err").toString();
        } else if (doc.object().keys().contains("ts")) {
            qCDebug(LG_PV_CONTROLLER) << "Received ts" << doc.object().value("ts").toInt();
        } else if (doc.object().keys().contains("mamps")) {
            m_parent->getController(CurrentController::CONTROLLER_NAME)->setValue(EnumsDeclarations::CURRENTS_PV, doc.object().value("mamps").toInt());
            m_parent->getController(CurrentController::CONTROLLER_NAME)->publishValue(EnumsDeclarations::CURRENTS_PV);
        } else if (doc.object().keys().contains("temp")) {
            m_parent->getController(TempController::CONTROLLER_NAME)->setValue(EnumsDeclarations::TEMPS_OUTSIDE2, doc.object().value("temp").toDouble());
            m_parent->getController(TempController::CONTROLLER_NAME)->publishValue(EnumsDeclarations::TEMPS_OUTSIDE2);
        } else if (doc.object().keys().contains("hum")) {
            m_parent->getController(HumidityController::CONTROLLER_NAME)->setValue(EnumsDeclarations::HUMIDITIES_OUTSIDE2, doc.object().value("hum").toDouble());
            m_parent->getController(HumidityController::CONTROLLER_NAME)->publishValue(EnumsDeclarations::HUMIDITIES_OUTSIDE2);
        }
    } else {
        qCWarning(LG_PV_CONTROLLER) << "Error while parsing json" << error.errorString();
    }
}
