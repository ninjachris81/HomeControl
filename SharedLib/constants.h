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

// ERRORS
#define MQTT_PATH_ERRORS "errors"
#define MQTT_PATH_ERRORS_RELAYS 0
#define ERRORS_LABEL_RELAYS "Relays"

// TEMPERATURES
#define MQTT_PATH_TEMPERATURES "temps"
#define MQTT_PATH_TEMPERATURES_INSIDE 0
#define TEMPERATURES_LABEL_INSIDE "Inside Temp"

// RELAYS
#define MQTT_PATH_RELAYS "relays"
#define MQTT_PATH_RELAYS_HEAT_PUMP 0
#define RELAYS_LABEL_HEAT_PUMP "Heat Pump"

#define MQTT_ID_DOUBLE 'd'
#define MQTT_ID_INTEGER 'i'
#define MQTT_ID_STRING 's'
#define MQTT_ID_BOOL 'b'
