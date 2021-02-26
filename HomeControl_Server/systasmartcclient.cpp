#include "systasmartcclient.h"
#include <QThread>
#include <QDateTime>

Q_LOGGING_CATEGORY(LG_SYSTA_SMART_C_CLIENT, "SSC Client")


SystaSmartCClient::SystaSmartCClient(AppConfiguration &appConfig, QObject *parent) : ModbusClient (appConfig, parent)
{
    connect(this, &SystaSmartCClient::clientConnected, this, &SystaSmartCClient::onConnected);
    connect(this, &SystaSmartCClient::clientDisonnected, this, &SystaSmartCClient::onDisconnected);
    connect(this, &SystaSmartCClient::receivedVariable, this, &SystaSmartCClient::onReceivedVariable);
    connect(this, &SystaSmartCClient::receivedHolding, [](SSC_HOLDINGS holding, QVariant value) {
        qDebug() << "Holding received" << holding << value;
    });

    connect(&m_requestTimer, &QTimer::timeout, this, &SystaSmartCClient::onCheckRequest);
    m_requestTimer.setInterval(1000);
}


void SystaSmartCClient::onConnected() {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO;

    Q_EMIT(connectionStateChanged(true));

    // init default
    QMapIterator<EnumsDeclarations::MQTT_SSC_VARIABLES, qint64> m_variableIterator(m_currentVariables);
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    while(m_variableIterator.hasNext()) {
        m_variableIterator.next();
        m_currentVariables.insert(m_variableIterator.key(), now);
        now+=200;           // add some spread
    }

    m_requestTimer.start();

    //qCDebug(LG_SYSTA_SMART_C_CLIENT) << readCoil(CIRC_ENABLE);
    //qCDebug(LG_SYSTA_SMART_C_CLIENT) << readCoil(CIRC_DISABLE);

    //readHolding(TEMP_TPU);
    //readVariable(STATUS_HK1);

    //qCDebug(LG_SYSTA_SMART_C_CLIENT) << readVariable(HK1_MAX_TEMP);
}

void SystaSmartCClient::onDisconnected() {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO;
    Q_EMIT(connectionStateChanged(false));
    m_requestTimer.stop();
}

void SystaSmartCClient::onCheckRequest() {
    QMapIterator<EnumsDeclarations::MQTT_SSC_VARIABLES, qint64> m_variableIterator(m_currentVariables);

    while(m_variableIterator.hasNext()) {
        m_variableIterator.next();
        qint64 timeout = m_variableMap.value(m_variableIterator.key());

        if (QDateTime::currentMSecsSinceEpoch() - m_variableIterator.value() > timeout) {
            readVariable(m_variableIterator.key());
        }
    }
}

bool SystaSmartCClient::waitForReply(QModbusReply* reply, int timeoutSec) {
    QDateTime started = QDateTime::currentDateTime();
    while(!reply->isFinished()) {
        QThread::currentThread()->msleep(10);
        if (started.secsTo(QDateTime::currentDateTime())>timeoutSec) {
            return false;
        }
    }

    return true;
}

void SystaSmartCClient::readCoil(SSC_COILS registerIndex) {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO << registerIndex;

    quint16 startAddress = registerIndex, numberOfRegisters = 1;
    QModbusRequest request(QModbusPdu::ReadCoils, startAddress, numberOfRegisters);

    QModbusReply* reply = sendRawRequest(request, SYSTA_SMART_C_CLIENT_ID);

    connect(reply, &QModbusReply::finished, [this, registerIndex, reply] {
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().isValid();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().size();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().isException();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().functionCode();

        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().data();
        Q_EMIT(receivedCoil(registerIndex, reply->rawResult().data().at(1) == 1 ? true : false));
    });
}

void SystaSmartCClient::readHolding(SSC_HOLDINGS registerIndex) {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO << registerIndex;

    quint16 startAddress = registerIndex, numberOfRegisters = 1;
    QModbusRequest request(QModbusPdu::ReadInputRegisters, startAddress, numberOfRegisters);

    QModbusReply* reply = sendRawRequest(request, SYSTA_SMART_C_CLIENT_ID);

    connect(reply, &QModbusReply::finished, [this, registerIndex, reply] {
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().isValid();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().size();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().isException();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().functionCode();

        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().data();
        Q_EMIT(receivedHolding(registerIndex, tempFrom8Bit(reply->rawResult().data().at(1), reply->rawResult().data().at(2))));
    });
}

void SystaSmartCClient::setupVariableRequest(EnumsDeclarations::MQTT_SSC_VARIABLES registerIndex, qint64 interval) {
    m_variableMap.insert(registerIndex, interval);
    m_currentVariables.insert(registerIndex, 0);
}

void SystaSmartCClient::readVariable(EnumsDeclarations::MQTT_SSC_VARIABLES registerIndex) {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO << registerIndex;

    quint16 startAddress = registerIndex;
    quint16 numberOfRegisters = 1;

    switch(registerIndex) {
    case EnumsDeclarations::SSC_SOLAR_POWER_TOTAL:
    case EnumsDeclarations::SSC_WATER_POWER:
    case EnumsDeclarations::SSC_CIRCULATION_POWER:
    case EnumsDeclarations::SSC_BOILER_LIFETIME:
    case EnumsDeclarations::SSC_BOILER_STARTS:
    case EnumsDeclarations::SSC_PELLETS_LIFETIME:
        numberOfRegisters = 2;
        break;
    default:
        break;
    }

    qCDebug(LG_SYSTA_SMART_C_CLIENT) << "Requesting" << registerIndex << numberOfRegisters;

    QModbusRequest request(QModbusPdu::ReadHoldingRegisters, startAddress, numberOfRegisters);

    QModbusReply* reply = sendRawRequest(request, SYSTA_SMART_C_CLIENT_ID);

    connect(reply, &QModbusReply::finished, [this, registerIndex, numberOfRegisters, reply] {
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().isValid();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().size();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().isException();
        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().functionCode();

        qCDebug(LG_SYSTA_SMART_C_CLIENT) << reply->rawResult().data();

        if (!reply->rawResult().isException()) {
            if (reply->rawResult().data().at(0) == numberOfRegisters * 2) {

                switch(registerIndex) {
                case EnumsDeclarations::SSC_SOLAR_POWER_TOTAL:
                case EnumsDeclarations::SSC_WATER_POWER:
                case EnumsDeclarations::SSC_CIRCULATION_POWER:
                case EnumsDeclarations::SSC_BOILER_LIFETIME:
                case EnumsDeclarations::SSC_BOILER_STARTS:
                case EnumsDeclarations::SSC_PELLETS_LIFETIME:
                    Q_EMIT(receivedVariable(registerIndex, QVariant::fromValue(ulongFrom8Bit(reply->rawResult().data().at(1), reply->rawResult().data().at(2), reply->rawResult().data().at(3), reply->rawResult().data().at(4)))));
                    break;
                case EnumsDeclarations::SSC_SMARTHOME_HK1_TARGET_TEMP:
                case EnumsDeclarations::SSC_SMARTHOME_HK2_TARGET_TEMP:
                case EnumsDeclarations::SSC_SMARTHOME_WATER_TARGET_TEMP:
                case EnumsDeclarations::SSC_HK1_MAX_TEMP:
                case EnumsDeclarations::SSC_HK2_MAX_TEMP:
                case EnumsDeclarations::SSC_BUFFER_TARGET_TEMP_TOP:
                case EnumsDeclarations::SSC_TANK_TARGET_TEMP:
                    Q_EMIT(receivedVariable(registerIndex, QVariant::fromValue(tempFrom8Bit(reply->rawResult().data().at(1), reply->rawResult().data().at(2)))));
                    break;
                default:
                    Q_EMIT(receivedVariable(registerIndex, QVariant::fromValue(intFrom8Bit(reply->rawResult().data().at(1), reply->rawResult().data().at(2)))));
                    break;
                }
            } else {
                qCWarning(LG_SYSTA_SMART_C_CLIENT) << "Unexpected size" << reply->rawResult().data().at(0);
            }
        } else {
            qCWarning(LG_SYSTA_SMART_C_CLIENT) << "Request returned exception";
        }
    });
}

void SystaSmartCClient::onReceivedVariable(EnumsDeclarations::MQTT_SSC_VARIABLES variable, QVariant value) {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO << variable << value;
    m_currentVariables.insert(variable, QDateTime::currentMSecsSinceEpoch());
}
