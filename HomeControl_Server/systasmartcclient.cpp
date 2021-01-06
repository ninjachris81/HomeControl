#include "systasmartcclient.h"
#include <QThread>
#include <QDateTime>

Q_LOGGING_CATEGORY(LG_SYSTA_SMART_C_CLIENT, "SSC Client")


SystaSmartCClient::SystaSmartCClient(AppConfiguration &appConfig, QObject *parent) : ModbusClient (appConfig, parent)
{
    connect(this, &SystaSmartCClient::clientConnected, this, &SystaSmartCClient::onConnected);
    connect(this, &SystaSmartCClient::receivedVariable, [](SSC_VARIABLES variable, QVariant value) {
        qDebug() << "Variable received" << variable << value;
    });
    connect(this, &SystaSmartCClient::receivedHolding, [](SSC_HOLDINGS holding, QVariant value) {
        qDebug() << "Holding received" << holding << value;
    });
}

void SystaSmartCClient::onConnected() {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO;

    //qCDebug(LG_SYSTA_SMART_C_CLIENT) << readCoil(CIRC_ENABLE);
    //qCDebug(LG_SYSTA_SMART_C_CLIENT) << readCoil(CIRC_DISABLE);

    readHolding(TEMP_TPU);
    //readVariable(BUFFER_TARGET_TEMP_TOP);

    //qCDebug(LG_SYSTA_SMART_C_CLIENT) << readVariable(HK1_MAX_TEMP);
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

void SystaSmartCClient::readVariable(SSC_VARIABLES registerIndex) {
    qCDebug(LG_SYSTA_SMART_C_CLIENT) << Q_FUNC_INFO << registerIndex;

    quint16 startAddress = registerIndex;
    quint16 numberOfRegisters = 1;

    switch(registerIndex) {
    case SOLAR_POWER_TOTAL:
    case WATER_POWER:
    case CIRCULATION_POWER:
    case BOILER_LIFETIME:
    case BOILER_STARTS:
    case PELLETS_LIFETIME:
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
                case SOLAR_POWER_TOTAL:
                case WATER_POWER:
                case CIRCULATION_POWER:
                case BOILER_LIFETIME:
                case BOILER_STARTS:
                case PELLETS_LIFETIME:
                    Q_EMIT(receivedVariable(registerIndex, QVariant::fromValue(ulongFrom8Bit(reply->rawResult().data().at(1), reply->rawResult().data().at(2), reply->rawResult().data().at(3), reply->rawResult().data().at(4)))));
                    break;
                case SMARTHOME_HK1_TARGET_TEMP:
                case SMARTHOME_HK2_TARGET_TEMP:
                case SMARTHOME_WATER_TARGET_TEMP:
                case HK1_MAX_TEMP:
                case HK2_MAX_TEMP:
                case BUFFER_TARGET_TEMP_TOP:
                case TANK_TARGET_TEMP:
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
