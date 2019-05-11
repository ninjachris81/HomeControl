#ifdef QT_VERSION
    #ifndef USES_CONSTANTS_QT
        #error Should use Qt version
    #endif
#endif

// hc/{topicName}/{mode}/{index}
// example:
// hc/relays/set/0
// hc/temps/val/0

#define MQTT_BASE_PATH "hc"
#define MQTT_MIN_MSG_SIZE 2

#define MQTT_PATH_SEP "/"
#define MQTT_WILDCARD "#"
#define MQTT_VAL "val"
#define MQTT_SET "set"
#define MQTT_LOG_SOURCE_DIV "> "

// DEVICE IDS
#define DEV_ID_SERVER "SERV"
#define DEV_ID_TERMINAL "TERM"
#define DEV_ID_4RELAY_BOARD "4RLB"
#define DEV_ID_WATER_TEMP_BOARD "WTB1"
#define DEV_ID_INSIDE_TEMP_BOARD "ITB1"
#define DEV_ID_SHOWER_REMOTE_CTRL "SHRC"

// BROADCASTS
#define MQTT_PATH_BC "bc"
#define MQTT_BC_CMD_BC_ALL "all"

// COMMANDS
#define MQTT_PATH_CMD "cmd"
#define MQTT_CMD_START_PREHEAT 0
#define MQTT_CMD_STOP_PREHEAT 1
#define MQTT_CMD_LOG_CLEAR_ALL 2
#define MQTT_CMD_LOG_CLEAR_INFO 3
#define MQTT_CMD_LOG_CLEAR_ERROR 4
#define MQTT_CMD_LOG_CLEAR_STARTUP 5

// LOGS
#define MQTT_PATH_LOGS "logs"

#define MQTT_PATH_LOGS_TYPE_INFO 0
#define LOGS_LABEL_INFO "Info"

#define MQTT_PATH_LOGS_TYPE_ERROR 1
#define LOGS_LABEL_ERROR "Error"

#define MQTT_PATH_LOGS_TYPE_STARTUP 2
#define LOGS_LABEL_STARTUP "Startup"

// SETTINGS
#define MQTT_PATH_SETTINGS "settings"

#define MQTT_PATH_SETTINGS_PREHEAT_FROM 0
#define SETTINGS_LABEL_PREHEAT_FROM "Preheat from"

#define MQTT_PATH_SETTINGS_PREHEAT_TO 1
#define SETTINGS_LABEL_PREHEAT_TO "Preheat to"

#define MQTT_PATH_SETTINGS_PREHEAT_HC_TEMP 2
#define SETTINGS_LABEL_PREHEAT_HC_TEMP "Preheat HC Temp"

#define MQTT_PATH_SETTINGS_PREHEAT_WATER_TEMP 3
#define SETTINGS_LABEL_PREHEAT_WATER_TEMP "Preheat Water Temp"

#define MQTT_PATH_SETTINGS_PREHEAT_MODE 4
#define SETTINGS_LABEL_PREHEAT_MODE "Preheat Mode"

#define MQTT_PATH_SETTINGS_PREHEAT_DURATION 5
#define SETTINGS_LABEL_PREHEAT_DURATION "Preheat Duration"

#define MQTT_PATH_SETTINGS_HEATING_TEMP 6
#define SETTINGS_LABEL_HEATING_TEMP "Heating Temp"

#define MQTT_PATH_SETTINGS_HEATING_USE_TOGGLE 7
#define SETTINGS_LABEL_HEATING_USE_TOGGLE "Use toggle"

#define MQTT_PATH_SETTINGS_HEATING_MODE 8
#define SETTINGS_LABEL_HEATING_MODE "Heating Mode"

#define MQTT_PATH_SETTINGS_HEATING_MANUAL_STATE 9
#define SETTINGS_LABEL_HEATING_MANUAL_STATE "Heating Manual State"

#define MQTT_PATH_SETTINGS_CORE_HOST 10
#define SETTINGS_LABEL_SETTINGS_CORE_HOST "Core Host"


// TEMPERATURES
#define MQTT_PATH_TEMPS "temps"

#define MQTT_PATH_TEMPS_HC 0
#define TEMPS_LABEL_HC "Heat Changer"

#define MQTT_PATH_TEMPS_TANK 1
#define TEMPS_LABEL_TANK "Tank"

#define MQTT_PATH_TEMPS_WATER 2
#define TEMPS_LABEL_WATER "Water"

#define MQTT_PATH_TEMPS_INSIDE 3
#define TEMPS_LABEL_INSIDE "Inside"

// RELAYS
#define MQTT_PATH_RELAYS "relays"

#define MQTT_PATH_RELAYS_HC_PUMP 0
#define RELAYS_LABEL_HC_PUMP "HC Pump"

#define MQTT_PATH_RELAYS_WATER_PUMP 1
#define RELAYS_LABEL_WATER_PUMP "Water Pump"

#define MQTT_PATH_RELAYS_HEATING_PUMP 2
#define RELAYS_LABEL_HEATING_PUMP "Heating Pump"

// TYPES
#define MQTT_ID_DOUBLE 'd'
#define MQTT_ID_INTEGER 'i'
#define MQTT_ID_STRING 's'
#define MQTT_ID_BOOL 'b'

// LIFETIMES
#define LIFETIME_UNLIMITED 0
#define LIFETIME_SHORT  10000           // 10 sec
#define LIFETIME_MID    120000          // 2 min
#define LIFETIME_LONG   900000          // 10 min
