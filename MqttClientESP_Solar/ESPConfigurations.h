#include "constants.h"

#define CONFIG_SOLAR_BOARD 0        // NodeMCU
#define CONFIG_INSIDE_TEMP_BOARD 1    // ESP
#define CONFIG_INSIDE_TEMP_BOARD_ALT 2    // NodeMCU EPS32S

#define TEMP_SENSOR_NONE 0
#define TEMP_SENSOR_DALLAS 1
#define TEMP_SENSOR_LM75 2

#define ACTIVE_CONFIG CONFIG_INSIDE_TEMP_BOARD_ALT

#define NO_CONNECTION_TIMEOUT_SEC 60
#define SEND_WAIT_TIME_MS 2000

#if ACTIVE_CONFIG == CONFIG_SOLAR_BOARD
  #warning Solar Board Configuration
  #define DEEP_SLEEP_DURATION 30e6
  #define TEMP_COUNT 0
  #define PIN_DIGITAL_TEMP_SENSORS 0
  #define DEVICE_NAME DEV_ID_SOLAR_SENSOR
  #define BRIGHTNESS_COUNT 1
  #define PIN_BRIGHTNESS A0
  #define TEMP_OFFSET 0
  #define REAL_DEEP_SLEEP 1
  #define TEMP_SENSOR TEMP_SENSOR_NONE
#elif ACTIVE_CONFIG == CONFIG_INSIDE_TEMP_BOARD
  #warning Temp Board Configuration
  #define DEEP_SLEEP_DURATION 10e6
  #define TEMP_COUNT 1
  #define PIN_DIGITAL_TEMP_SENSORS 2
  #define DEVICE_NAME DEV_ID_SOLAR_SENSOR
  #define BRIGHTNESS_COUNT 0
  #define PIN_BRIGHTNESS 0
  #define TEMP_OFFSET 0
  #define REAL_DEEP_SLEEP 0
  #define TEMP_SENSOR TEMP_SENSOR_DALLAS
#elif ACTIVE_CONFIG == CONFIG_INSIDE_TEMP_BOARD_ALT
  #warning Temp Board Alt Configuration
  #define DEEP_SLEEP_DURATION 20e6
  #define TEMP_COUNT 1
  #define PIN_DIGITAL_TEMP_SENSORS 2
  #define DEVICE_NAME DEV_ID_SOLAR_SENSOR
  #define BRIGHTNESS_COUNT 0
  #define PIN_BRIGHTNESS 0
  #define TEMP_OFFSET 0
  #define REAL_DEEP_SLEEP 1
  #define TEMP_SENSOR TEMP_SENSOR_LM75
#endif
