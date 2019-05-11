#ifndef CONSTANTS_QT_H
#define CONSTANTS_QT_H

#define USES_CONSTANTS_QT

#include "constants.h"
#include <QObject>

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

// LOGS
enum MQTT_LOGS {
    LOGS_TYPE_INFO = MQTT_PATH_LOGS_TYPE_INFO,
    LOGS_TYPE_ERROR = MQTT_PATH_LOGS_TYPE_ERROR,
    LOGS_TYPE_STARTUP = MQTT_PATH_LOGS_TYPE_STARTUP
};

static QString LOGS_TYPE_LABELS[] = {
    LOGS_LABEL_INFO,
    LOGS_LABEL_ERROR,
    LOGS_LABEL_STARTUP
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
    SETTINGS_CORE_HOST = MQTT_PATH_SETTINGS_CORE_HOST
};

Q_ENUMS(MQTT_SETTINGS)

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
    SETTINGS_LABEL_SETTINGS_CORE_HOST
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
    TEMPS_INSIDE = MQTT_PATH_TEMPS_INSIDE
};
Q_ENUMS(MQTT_TEMPS)

static QString TEMPS_LABELS[] = {
    TEMPS_LABEL_HC,
    TEMPS_LABEL_TANK,
    TEMPS_LABEL_WATER,
    TEMPS_LABEL_INSIDE
};

// RELAYS
enum MQTT_RELAYS {
    RELAYS_HC_PUMP = MQTT_PATH_RELAYS_HC_PUMP,
    RELAYS_WATER_PUMP = MQTT_PATH_RELAYS_WATER_PUMP,
    RELAYS_HEATING_PUMP = MQTT_PATH_RELAYS_HEATING_PUMP
};
Q_ENUMS(MQTT_RELAYS)

static QString RELAYS_LABELS[] {
    RELAYS_LABEL_HC_PUMP,
    RELAYS_LABEL_WATER_PUMP,
    RELAYS_LABEL_HEATING_PUMP
};

}

#endif // CONSTANTS_QT_H
