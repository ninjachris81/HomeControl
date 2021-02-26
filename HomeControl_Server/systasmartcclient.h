#ifndef SYSTASMARTCCLIENT_H
#define SYSTASMARTCCLIENT_H

#include <QObject>
#include <QLoggingCategory>
#include <QTimer>
#include <QMap>
#include <QMapIterator>

#include "modbusclient.h"
#include "controller/ssccontroller.h"

Q_DECLARE_LOGGING_CATEGORY(LG_SYSTA_SMART_C_CLIENT)

#define SYSTA_SMART_C_CLIENT_ID 1

class SystaSmartCClient : public ModbusClient
{
    Q_OBJECT

public:
    enum SSC_COILS {
        SMARTHOME_ENABLED = 0,
        HK1_AVAILABLE,
        HK2_AVAILABLE,
        DHW_ENABLE = 4,
        DHW_DISABLE,
        CIRC_ENABLE,
        CIRC_DISABLE
    };

    enum SSC_HOLDINGS {
        TEMP_TA = 0,
        TEMP_TV,
        TEMP_SHK1,
        TEMP_TWO,
        TEMP_TPO,
        TEMP_TPU,
        TEMP_TZR,
        TEMP_TV2,
        TEMP_SHK2,
        TEMP_TR,
        TEMP_TR2,
        TEMP_TSA,
        TEMP_TANK_FEED,
        TEMP_TANK_RETURN,
        TEMP_WOOD_TANK_FEED,
        TEMP_WOOD_TANK_RETURN,
        TEMP_WOOD_TANK_TOP,
        TEMP_SWIMMING_POOL = 19,
        TEMP_TVSB,
        TEMP_TRSB
    };

    explicit SystaSmartCClient(AppConfiguration &appConfig, QObject *parent = nullptr);

    void readCoil(SSC_COILS registerIndex);

    void readHolding(SSC_HOLDINGS registerIndex);

    void readVariable(EnumsDeclarations::MQTT_SSC_VARIABLES registerIndex);

    void setupVariableRequest(EnumsDeclarations::MQTT_SSC_VARIABLES registerIndex, qint64 interval);

private:
    QTimer m_requestTimer;
    QMap<EnumsDeclarations::MQTT_SSC_VARIABLES, qint64> m_variableMap;
    QMap<EnumsDeclarations::MQTT_SSC_VARIABLES, qint64> m_currentVariables;

    bool waitForReply(QModbusReply *reply, int timeoutSec);

    inline qulonglong ulongFrom8Bit(uint8_t high1, uint8_t low1, uint8_t high2, uint8_t low2) {
        qDebug() << high1 << low1 << high2 << low2;

        return static_cast<qulonglong>(high1 << 24) + static_cast<qulonglong>(low1 << 16) + static_cast<qulonglong>(high2 << 8) + static_cast<qulonglong>(low2);
    }

    inline qint64 longFrom8Bit(uint8_t high1, uint8_t low1, uint8_t high2, uint8_t low2) {
        return (qint64) (high1 << 24) | (low1 << 16) | (high2 << 8) | low2;
    }

    inline int intFrom8Bit(uint8_t high, uint8_t low)
    {
        return (static_cast<qint16>(high) << 8) | low;
    }

    inline float tempFrom8Bit(uint8_t high, uint8_t low) {
        float v = intFrom8Bit(high, low);
        return v / 10;
    }


private slots:
    void onConnected();
    void onDisconnected();
    void onCheckRequest();

    void onReceivedVariable(EnumsDeclarations::MQTT_SSC_VARIABLES variable, QVariant value);

signals:
    void connectionStateChanged(bool connected);
    void receivedCoil(SSC_COILS coil, bool value);
    void receivedHolding(SSC_HOLDINGS holding, float value);
    void receivedVariable(EnumsDeclarations::MQTT_SSC_VARIABLES variable, QVariant value);

public slots:
};

#endif // SYSTASMARTCCLIENT_H
