
#define CONFIG_4_RELAY_BOARD 0        // ESP
#define CONFIG_TEMP_BOARD 1           // D1 R1
#define CONFIG_INSIDE_TEMP_BOARD 2    // ESP


#define ACTIVE_CONFIG CONFIG_INSIDE_TEMP_BOARD



#if ACTIVE_CONFIG == CONFIG_4_RELAY_BOARD
  #warning Relay Board Configuration
  #define RELAY_COUNT 3
  #define TEMP_COUNT 0
  #define PIN_DIGITAL_TEMP_SENSORS 0
//  #define MQTT_NAME "Relay Board"
  #define DEVICE_NAME DEV_ID_4RELAY_BOARD
  #define TEMP_OFFSET 0
  #define TEMP_ADDRESS_MAPPING_0 {}
  #define TEMP_ADDRESS_MAPPING_1 {}
  #define TEMP_ADDRESS_MAPPING_2 {}
  #define TEMP_ADDRESS_MAPPING_3 {}
#elif ACTIVE_CONFIG == CONFIG_TEMP_BOARD
  #warning Temp Board Configuration
  #define RELAY_COUNT 0
  #define TEMP_COUNT 3
  #define PIN_DIGITAL_TEMP_SENSORS D3
//  #define MQTT_NAME "Temp Board"
  #define DEVICE_NAME DEV_ID_WATER_TEMP_BOARD
  #define TEMP_OFFSET 0
  #define TEMP_ADDRESS_MAPPING_0 {0x28,0xff,0x4c,0x01,0x67,0x14,0x02,0xbb}
  #define TEMP_ADDRESS_MAPPING_1 {0x28,0xff,0xbf,0x12,0x67,0x14,0x02,0xe3}
  #define TEMP_ADDRESS_MAPPING_2 {0x28,0xff,0x9a,0x13,0x67,0x14,0x02,0x46}
  #define TEMP_ADDRESS_MAPPING_3 {}
#elif ACTIVE_CONFIG == CONFIG_INSIDE_TEMP_BOARD
  #warning Inside Temp Board Configuration
  #define RELAY_COUNT 0
  #define TEMP_COUNT 1
  #define PIN_DIGITAL_TEMP_SENSORS 2
//  #define MQTT_NAME "Inside Temp Board"
  #define DEVICE_NAME DEV_ID_INSIDE_TEMP_BOARD
  #define TEMP_OFFSET 3
  #define TEMP_ADDRESS_MAPPING_0 {0x28,0xff,0x5f,0xff,0x66,0x14,0x02,0x46}
  #define TEMP_ADDRESS_MAPPING_1 {}
  #define TEMP_ADDRESS_MAPPING_2 {}
  #define TEMP_ADDRESS_MAPPING_3 {}
#endif

#if RELAY_COUNT == 0 && TEMP_COUNT == 0
  #error No valid configuration
#endif
