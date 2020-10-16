#ifndef CONSTANTS_QT_H
#define CONSTANTS_QT_H

#define USES_CONSTANTS_QT

#include "constants.h"
#include <QObject>
#include <QVariant>

namespace EnumsDeclarations {
Q_NAMESPACE

// COMMANDS
enum MQTT_CMDS {
    CMD_NONE = -1,
    CMD_START_PREHEAT = MQTT_CMD_START_PREHEAT,
    CMD_STOP_PREHEAT = MQTT_CMD_STOP_PREHEAT,
    CMD_LOG_CLEAR_ALL = MQTT_CMD_LOG_CLEAR_ALL,
    CMD_LOG_CLEAR_INFO = MQTT_CMD_LOG_CLEAR_INFO,
    CMD_LOG_CLEAR_ERROR = MQTT_CMD_LOG_CLEAR_ERROR,
    CMD_LOG_CLEAR_STARTUP = MQTT_CMD_LOG_CLEAR_STARTUP
};
Q_ENUMS(MQTT_CMDS)

// GENERAL TYPES
enum VALIDATED_BOOL {
    BOOL_INVALID,
    BOOL_TRUE,
    BOOL_FALSE
};
Q_ENUMS(VALIDATED_BOOL)

// LOGS
enum MQTT_LOGS {
    LOGS_TYPE_INFO = MQTT_PATH_LOGS_TYPE_INFO,
    LOGS_TYPE_ERROR = MQTT_PATH_LOGS_TYPE_ERROR,
    LOGS_TYPE_STARTUP = MQTT_PATH_LOGS_TYPE_STARTUP
};
Q_ENUMS(MQTT_LOGS)

static QString LOGS_TYPE_LABELS[] = {
    LOGS_LABEL_INFO,
    LOGS_LABEL_ERROR,
    LOGS_LABEL_STARTUP
};

// INFOS
enum MQTT_INFOS {
    INFOS_SYSTEM_TIME = MQTT_PATH_INFOS_SYSTEM_TIME,
    INFOS_SYSTEM_TEMP = MQTT_PATH_INFOS_SYSTEM_TEMP,
    INFOS_BOILER_TARGET_TEMP = MQTT_PATH_INFOS_BOILER_TARGET_TEMP,
    INFOS_SUN_EXPECTED = MQTT_PATH_INFOS_SUN_EXPECTED,
    INFOS_AVG_TEMP_FORECAST = MQTT_PATH_INFOS_AVG_TEMP_FORECAST,
    INFOS_AVG_CLOUDS_FORECAST = MQTT_PATH_INFOS_AVG_CLOUDS_FORECAST
};

Q_ENUMS(MQTT_INFOS)

static QString INFOS_LABELS[] = {
    INFOS_LABEL_SYSTEM_TIME,
    INFOS_LABEL_SYSTEM_TEMP,
    INFOS_LABEL_BOILER_TARGET_TEMP,
    INFOS_LABEL_SUN_EXPECTED,            // validated bool
    INFOS_LABEL_AVG_TEMP_FORECAST,
    INFOS_LABEL_AVG_CLOUDS_FORECAST
};

// SETTINGS
enum MQTT_SETTINGS {
    SETTINGS_PREHEAT_FROM = MQTT_PATH_SETTINGS_PREHEAT_FROM,
    SETTINGS_PREHEAT_TO = MQTT_PATH_SETTINGS_PREHEAT_TO,
    SETTINGS_PREHEAT_HC_TEMP = MQTT_PATH_SETTINGS_PREHEAT_HC_TEMP,
    SETTINGS_PREHEAT_WATER_TEMP = MQTT_PATH_SETTINGS_PREHEAT_WATER_TEMP,
    SETTINGS_PREHEAT_MODE = MQTT_PATH_SETTINGS_PREHEAT_MODE,
    SETTINGS_PREHEAT_DURATION = MQTT_PATH_SETTINGS_PREHEAT_DURATION,
    SETTINGS_HEATING_TEMP = MQTT_PATH_SETTINGS_HEATING_TEMP,
    SETTINGS_HEATING_USE_TOGGLE = MQTT_PATH_SETTINGS_HEATING_USE_TOGGLE,
    SETTINGS_HEATING_MODE = MQTT_PATH_SETTINGS_HEATING_MODE,
    SETTINGS_HEATING_MANUAL_STATE = MQTT_PATH_SETTINGS_HEATING_MANUAL_STATE,

    SETTINGS_CORE_HOST = MQTT_PATH_SETTINGS_CORE_HOST,
    SETTINGS_PREHEAT_HC_STANDBY_TEMP = MQTT_PATH_SETTINGS_PREHEAT_HC_STANDBY_TEMP,
    SETTINGS_PREHEAT_STANDBY_FROM = MQTT_PATH_SETTINGS_PREHEAT_STANDBY_FROM,
    SETTINGS_PREHEAT_STANDBY_TO = MQTT_PATH_SETTINGS_PREHEAT_STANDBY_TO,
    SETTINGS_HEATING_MONTH_FROM = MQTT_PATH_SETTINGS_HEATING_MONTH_FROM,
    SETTINGS_HEATING_MONTH_TO = MQTT_PATH_SETTINGS_HEATING_MONTH_TO,
    SETTINGS_HEATING_TOGGLE_ON_DURATION = MQTT_PATH_SETTINGS_HEATING_TOGGLE_ON_DURATION,
    SETTINGS_HEATING_HOUR_FROM = MQTT_PATH_SETTINGS_HEATING_HOUR_FROM,
    SETTINGS_HEATING_HOUR_TO = MQTT_PATH_SETTINGS_HEATING_HOUR_TO,
    SETTINGS_HEATING_MIN_TEMP_TANK = MQTT_PATH_SETTINGS_HEATING_MIN_TEMP_TANK,

    SETTINGS_TANK_OFFSET = MQTT_PATH_SETTINGS_TANK_OFFSET,
    SETTINGS_TEMP_EXP_THRESHOLD = MQTT_PATH_SETTINGS_TEMP_EXP_THRESHOLD,
    SETTINGS_CLOUDS_EXP_THRESHOLD = MQTT_PATH_SETTINGS_CLOUDS_EXP_THRESHOLD,
    SETTINGS_TANK_MIN_TEMP = MQTT_PATH_SETTINGS_TANK_MIN_TEMP,
    SETTINGS_BOILER_SCHEDULE = MQTT_PATH_SETTINGS_BOILER_SCHEDULE,
    SETTINGS_SUN_EXP_SUBSTRACT = MQTT_PATH_SETTINGS_SUN_EXP_SUBSTRACT
};

Q_ENUMS(MQTT_SETTINGS)

static QVariant::Type
#ifdef __GNUG__
__attribute__((used))
#endif
SETTINGS_TYPES[] =  {
    QVariant::Int,
    QVariant::Int,
    QVariant::Double,
    QVariant::Double,
    QVariant::Int,
    QVariant::Int,
    QVariant::Double,
    QVariant::Bool,
    QVariant::Int,
    QVariant::Bool,

    QVariant::String,
    QVariant::Double,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,

    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::Int,
    QVariant::String,
    QVariant::Int
};

static QString SETTINGS_LABELS[] = {
    SETTINGS_LABEL_PREHEAT_FROM,
    SETTINGS_LABEL_PREHEAT_TO,
    SETTINGS_LABEL_PREHEAT_HC_TEMP,
    SETTINGS_LABEL_PREHEAT_WATER_TEMP,
    SETTINGS_LABEL_PREHEAT_MODE,
    SETTINGS_LABEL_PREHEAT_DURATION,
    SETTINGS_LABEL_HEATING_TEMP,
    SETTINGS_LABEL_HEATING_USE_TOGGLE,
    SETTINGS_LABEL_HEATING_MODE,
    SETTINGS_LABEL_HEATING_MANUAL_STATE,

    SETTINGS_LABEL_CORE_HOST,
    SETTINGS_LABEL_PREHEAT_HC_STANDBY_TEMP,
    SETTINGS_LABEL_PREHEAT_STANDBY_FROM,
    SETTINGS_LABEL_PREHEAT_STANDBY_TO,
    SETTINGS_LABEL_HEATING_MONTH_FROM,
    SETTINGS_LABEL_HEATING_MONTH_TO,
    SETTINGS_LABEL_HEATING_TOGGLE_ON_DURATION,
    SETTINGS_LABEL_HEATING_HOUR_FROM,
    SETTINGS_LABEL_HEATING_HOUR_TO,
    SETTINGS_LABEL_HEATING_MIN_TEMP_TANK,

    SETTINGS_LABEL_TANK_OFFSET,
    SETTINGS_LABEL_TEMP_EXP_THRESHOLD,
    SETTINGS_LABEL_CLOUDS_EXP_THRESHOLD,
    SETTINGS_LABEL_TANK_MIN_TEMP,
    SETTINGS_LABEL_BOILER_SCHEDULE,
    SETTINGS_LABEL_SUN_EXP_SUBSTRACT
};

enum SETTINGS_MODES {
    SETTING_MODE_AUTOMATIC,
    SETTING_MODE_MANUAL
};

Q_ENUMS(SETTINGS_MODES)

// TEMPERATURES
enum MQTT_TEMPS {
    TEMPS_HC = MQTT_PATH_TEMPS_HC,
    TEMPS_TANK = MQTT_PATH_TEMPS_TANK,
    TEMPS_WATER = MQTT_PATH_TEMPS_WATER,
    TEMPS_SOLAR_HC = MQTT_PATH_TEMPS_SOLAR_HC,
    TEMPS_INSIDE = MQTT_PATH_TEMPS_INSIDE,
    TEMPS_OUTSIDE = MQTT_PATH_TEMPS_OUTSIDE,
    TEMPS_OUTSIDE2 = MQTT_PATH_TEMPS_OUTSIDE2
};
Q_ENUMS(MQTT_TEMPS)

static QString TEMPS_LABELS[] = {
    TEMPS_LABEL_HC,
    TEMPS_LABEL_TANK,
    TEMPS_LABEL_WATER,
    TEMPS_LABEL_SOLAR_HC,
    TEMPS_LABEL_INSIDE,
    TEMPS_LABEL_OUTSIDE,
    TEMPS_LABEL_OUTSIDE2
};

// RELAYS
enum MQTT_RELAYS {
    RELAYS_HC_PUMP = MQTT_PATH_RELAYS_HC_PUMP,
    RELAYS_WATER_PUMP = MQTT_PATH_RELAYS_WATER_PUMP,
    RELAYS_HEATING_PUMP = MQTT_PATH_RELAYS_HEATING_PUMP,
    RELAYS_BOILER = MQTT_PATH_RELAYS_BOILER,
    RELAYS_SOLAR_PUMP = MQTT_PATH_RELAYS_SOLAR_PUMP,
    RELAYS_LIGHT_OUTSIDE = MQTT_PATH_RELAYS_LIGHT_OUTSIDE
};
Q_ENUMS(MQTT_RELAYS)

static QString RELAYS_LABELS[] {
    RELAYS_LABEL_HC_PUMP,
    RELAYS_LABEL_WATER_PUMP,
    RELAYS_LABEL_HEATING_PUMP,
    RELAYS_LABEL_BOILER,
    RELAYS_LABEL_SOLAR_PUMP,
    RELAYS_LABEL_LIGHT_OUTSIDE
};

enum MQTT_BRIGHTNESSES {
    BRIGHTNESSES_SOLAR = MQTT_PATH_BRIGHTNESSES_SOLAR,
    BRIGHTNESSES_OUTSIDE = MQTT_PATH_BRIGHTNESSES_OUTSIDE
};
Q_ENUMS(MQTT_BRIGHTNESSES)

static QString BRIGHTNESSES_LABELS[] {
    BRIGHTNESSES_LABEL_SOLAR,
    BRIGHTNESSES_LABEL_OUTSIDE
};

enum MQTT_HUMIDITIES {
    HUMIDITIES_OUTSIDE = MQTT_PATH_HUMIDITIES_OUTSIDE,
    HUMIDITIES_OUTSIDE2 = MQTT_PATH_HUMIDITIES_OUTSIDE2
};
Q_ENUMS(MQTT_HUMIDITIES)

static QString HUMIDITIES_LABELS[] {
    HUMIDITIES_LABEL_OUTSIDE,
    HUMIDITIES_LABEL_OUTSIDE2
};

enum MQTT_CURRENTS {
    CURRENTS_MAIN_BASEMENT = MQTT_PATH_CURRENTS_MAIN_BASEMENT,
    CURRENTS_PV = MQTT_PATH_CURRENTS_PV
};
Q_ENUMS(MQTT_CURRENTS)

static QString CURRENTS_LABELS[] {
    CURRENTS_LABEL_MAIN_BASEMENT,
    CURRENTS_LABEL_PV
};

enum MQTT_SWITCHES {
    SWITCHES_WINTERGARDEN = MQTT_PATH_SWITCHES_WINTERGARDEN,
    SWITCHES_PIR = MQTT_PATH_SWITCHES_PIR
};
Q_ENUMS(MQTT_SWITCHES)

static QString SWITCHES_LABELS[] {
    SWITCHES_LABEL_WINTERGARDEN,
    SWITCHES_LABEL_PIR
};

// EXTERNAL PV
enum MQTT_PVS {
    PVS_MAMPS = MQTT_PATH_PVS_MAMPS,
    PVS_TEMP = MQTT_PATH_PVS_TEMP,
    PVS_HUM = MQTT_PATH_PVS_HUM
};
Q_ENUMS(MQTT_PVS)

static QString PVS_LABELS[] {
    PVS_LABEL_MAMPS,
    PVS_LABEL_TEMP,
    PVS_LABEL_HUM
};

}

#endif // CONSTANTS_QT_H
