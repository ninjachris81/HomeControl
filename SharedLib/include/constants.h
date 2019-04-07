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

// BROADCASTS
#define MQTT_PATH_BC "bc"
#define MQTT_BC_CMD_BC_ALL "all"

// COMMANDS
#define MQTT_PATH_CMD "cmd"
#define MQTT_CMD_START_PREHEAT 0

// ERRORS
#define MQTT_PATH_ERRORS "errors"

#define MQTT_PATH_ERRORS_RELAYS 0
#define ERRORS_LABEL_RELAYS "Relays"

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

// TEMPERATURES
#define MQTT_PATH_TEMPS "temps"

#define MQTT_PATH_TEMPS_HC 0
#define TEMPS_LABEL_HC "Heat Changer"

#define MQTT_PATH_TEMPS_TANK 1
#define TEMPS_LABEL_TANK "Tank"

#define MQTT_PATH_TEMPS_WATER 2
#define TEMPS_LABEL_WATER "Water"

// RELAYS
#define MQTT_PATH_RELAYS "relays"

#define MQTT_PATH_RELAYS_HC_PUMP 0
#define RELAYS_LABEL_HC_PUMP "HC Pump"

#define MQTT_PATH_RELAYS_PREHEAT_PUMP 1
#define RELAYS_LABEL_PREHEAT_PUMP "Preheat Pump"

// TYPES
#define MQTT_ID_DOUBLE 'd'
#define MQTT_ID_INTEGER 'i'
#define MQTT_ID_STRING 's'
#define MQTT_ID_BOOL 'b'
