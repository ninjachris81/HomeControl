#ifndef SSCCONTROLLER_H
#define SSCCONTROLLER_H

#include "controllerbase.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_SSC_CONTROLLER)


class SSCController : public ControllerBase
{
    Q_OBJECT
public:

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

    explicit SSCController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return SSC_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getDefaultValueType();

    QString getEnumName();

    qint64 getValueLifetime(int index = -1);

    VALUE_BC_INTERVAL getValueBCInterval(int index=-1);

    static QString CONTROLLER_NAME;

    bool isValueOwner(int index = -1);

private:

protected:
    void onInit();

    void onValueChanged(int index, QVariant value);

};

#endif // SSCCONTROLLER_H
