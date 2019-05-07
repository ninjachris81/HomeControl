
#define CONFIG_4_RELAY_BOARD 0        // ESP
#define CONFIG_TEMP_BOARD 1           // D1 R1
#define CONFIG_INSIDE_TEMP_BOARD 2    // ESP


#define ACTIVE_CONFIG CONFIG_TEMP_BOARD



#if ACTIVE_CONFIG == CONFIG_4_RELAY_BOARD
  #warning Relay Board Configuration
  #define RELAY_COUNT 4
  #define TEMP_COUNT 0
  #define PIN_DIGITAL_TEMP_SENSORS 0
  #define MQTT_NAME "Relay Board"
  #define TEMP_OFFSET 0
#elif ACTIVE_CONFIG == CONFIG_TEMP_BOARD
  #warning Temp Board Configuration
  #define RELAY_COUNT 0
  #define TEMP_COUNT 3
  #define PIN_DIGITAL_TEMP_SENSORS D3
  #define MQTT_NAME "Temp Board"
  #define TEMP_OFFSET 0
#elif ACTIVE_CONFIG == CONFIG_INSIDE_TEMP_BOARD
  #warning Inside Temp Board Configuration
  #define RELAY_COUNT 0
  #define TEMP_COUNT 1
  #define PIN_DIGITAL_TEMP_SENSORS 2
  #define MQTT_NAME "Inside Temp Board"
  #define TEMP_OFFSET 3
#endif

#if RELAY_COUNT == 0 && TEMP_COUNT == 0
  #error No valid configuration
#endif
