
#define CONFIG_4_RELAY_BOARD 0
#define CONFIG_TEMP_BOARD 1



#define ACTIVE_CONFIG CONFIG_4_RELAY_BOARD



#if ACTIVE_CONFIG == CONFIG_4_RELAY_BOARD
  #warning Relay Board Configuration
  #define RELAY_COUNT 4
  #define TEMP_COUNT 0
  #define PIN_DIGITAL_TEMP_SENSORS 0
#elif ACTIVE_CONFIG == CONFIG_TEMP_BOARD
  #warning Temp Board Configuration
  #define RELAY_COUNT 0
  #define TEMP_COUNT 1
  #define PIN_DIGITAL_TEMP_SENSORS D3
#endif

#if RELAY_COUNT == 0 && TEMP_COUNT == 0
  #error No valid configuration
#endif
