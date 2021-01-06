#ifndef SYSTASMARTCCLIENT_H
#define SYSTASMARTCCLIENT_H

#include <QObject>
#include "modbusclient.h"

Q_DECLARE_LOGGING_CATEGORY(LG_SYSTA_SMART_C_CLIENT)

#define SYSTA_SMART_C_CLIENT_ID 1

class SystaSmartCClient : public ModbusClient
{
    Q_OBJECT

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

    enum SSC_VARIABLES {
        SMARTHOME_TYPE = 0,
        SMARTHOME_ERROR_CODE,
        SMARTHOME_HK1_TARGET_TEMP,      // temp
        SMARTHOME_HK2_TARGET_TEMP,      // temp
        SMARTHOME_WATER_TARGET_TEMP = 8,
        HK1_MAX_TEMP,                   // temp
        HK2_MAX_TEMP,                   // temp
        ERROR_CODE_CONTROLLER = 12,
        ERROR_CODE_SOLAR,
        ERROR_CODE_MODULA2,
        ERROR_CODE_MODULA3,
        ERROR_CODE_MODULA_PMI,
        ERROR_CODE_PELLET_TI3,
        ERROR_CODE_PELLET_TOUCH,
        SOLAR_POWER_CURRENT,
        SOLAR_POWER_DAY,
        SOLAR_POWER_TOTAL,
        WATER_POWER = 23,               // kwh
        CIRCULATION_POWER = 25,         // kwh
        BOILER_LIFETIME = 27,           // hours
        BOILER_STARTS = 29,             // count
        PELLETS_LIFETIME = 31,
        PELLETS_CONSUMPTION = 33,
        STATUS_WATER,                   // SSC_WATER_STATUS
        STATUS_CIRCULATION,             // SSC_CIRCULATION_STATUS
        STATUS_HK1,                     // SSC_HEATING_STATUS
        STATUS_HK2,                     // SSC_HEATING_STATUS
        STATUS_SOLAR = 39,              // SCC_SOLAR_STATUS
        STATUS_SWIMMING_POOL,           // SCC_SWIMMING_POOL_STATUS
        STATUS_BOILER,                  // SCC_BOILER_STATUS
        STATUS_PELLET,                  // SCC_PELLET_STATUS
        STATUS_WOOD,                    // SCC_WOOD_BOILER_STATUS
        BUFFER_TARGET_TEMP_TOP,         // temp
        TANK_TARGET_TEMP                // temp
    };

    enum SSC_WATER_STATUS {
        WS_WARM_WATER_NOT_REQUIRED,
        WS_WARMING_UP,
        WS_FROST_PROTECTION_ACTIVE,
        WS_WARM_WATER_REQUIRED_BUT_BLOCKED,
        WS_PUMP_LAG,
        WS_TANK_TOO_HOT,
        WS_WAITING_FOR_WATER_USAGE,
        WS_WATER_USAGE,
        WS_SETUP,
        WS_MANUAL_OPERATION,
        WS_CIRCULATION,
        WS_CIRCULATION_LAG,
        WS_CIRCULATION_BLOCKED,
        WS_HEATING_BLOCKED_BY_SMARTHOME
    };

    enum SSC_CIRCULATION_STATUS {
        CS_NOT_IN_USE,
        CS_LAG,
        CS_BLOCKED,
        CS_OFF,
        CS_BLOCKED_BY_TEMP_SENSOR,
        CS_ON,
        CS_FROST_PROTECTION,
        CS_BLOCKED_BY_SMARTHOME
    };

    enum SSC_HEATING_STATUS {
        HC_OFF,
        HC_ON,
        HC_NOT_REQUIRED,
        HC_PREPARATION,
        HC_BLOCKED,
        HC_SETUP,
        HC_FROST_PROTECTION,
        HC_SCREED,
        HC_OVERHEAD_COOLING,
        HC_MANUAL_OPERATION,
        HC_EMERGENCY_MODE,
        HC_NOT_INSTALLED,
        HC_COOLING
    };

    enum SCC_SWIMMING_POOL_STATUS {
        SPS_NOT_FOUND,
        SPS_OFF,
        SPS_BLOCKED,
        SPS_OK,
        SPS_FROST_PROTECTION,
        SPS_NORMAL,
        SPS_COMFORT,
        SPS_OVERHEAD_FEEDING,
        SPS_BLOCKED_TANK_TOO_COLD,
        SPS_BLOCKED_WATER_PREPARATION,
        SPS_COOLING
    };

    enum SCC_BOILER_STATUS {
        BS_OFF,
        BS_ON,
        BS_WATER_PREPARATION,
        BS_HEATING,
        BS_LOADING_TANK,
        BS_BLOCKED,
        BS_COOLING,
        BS_MODULA_NT_WATER_PREPARATION
    };

    enum SCC_PELLET_STATUS {
        PS_OFF,
        PS_STANDBY,
        PS_WARMING_UP,
        PS_RUNNING,
        PS_EXHAUST_TEST,
        PS_LAG,
        PS_CLEANING,
        PS_ERROR,
        PS_UNKNOWN
    };

    enum SCC_WOOD_BOILER_STATUS {
        WBS_NOT_FOUND,
        WBS_OFF,
        WBS_WARMING_UP,
        WBS_RUNNING,
        WBS_LAG,
        WBS_COOLING,
        WBS_SHUTTING_DOWN,
        WBS_BOOST
    };

    enum SCC_SOLAR_STATUS {
        SS_WAITING_FOR_SUN,
        SS_FROST_PROTECTION,
        SS_PUSH,
        SS_LAG,
        SS_HEATING,
        SS_FULL,
        SS_OVERHEAT,
        SS_MANUAL_OPERATION,
        SS_READING,
        SS_EMERGENCY_MODE
    };

public:
    explicit SystaSmartCClient(AppConfiguration &appConfig, QObject *parent = nullptr);

    void readCoil(SSC_COILS registerIndex);

    void readHolding(SSC_HOLDINGS registerIndex);

    void readVariable(SSC_VARIABLES registerIndex);

private:
    bool waitForReply(QModbusReply *reply, int timeoutSec);

    inline qulonglong ulongFrom8Bit(uint8_t high1, uint8_t low1, uint8_t high2, uint8_t low2) {
        qDebug() << high1 << low1 << high2 << low2;

        return static_cast<qulonglong>(high1 << 24) + static_cast<qulonglong>(low1 << 16) + static_cast<qulonglong>(high2 << 8) + static_cast<qulonglong>(low2);
    }

    inline qint64 longFrom8Bit(uint8_t high1, uint8_t low1, uint8_t high2, uint8_t low2) {
        return (qint64) (high1 << 24) | (low1 << 16) | (high2 << 8) | low2;
    }

    inline qint16 intFrom8Bit(uint8_t high, uint8_t low)
    {
        return (static_cast<qint16>(high) << 8) | low;
    }

    inline float tempFrom8Bit(uint8_t high, uint8_t low) {
        float v = intFrom8Bit(high, low);
        return v / 10;
    }


private slots:
    void onConnected();

signals:
    void receivedCoil(SSC_COILS coil, bool value);
    void receivedHolding(SSC_HOLDINGS holding, float value);
    void receivedVariable(SSC_VARIABLES variable, QVariant value);

public slots:
};

#endif // SYSTASMARTCCLIENT_H
