
#define CONFIG_4_RELAY_BOARD 0        // Generic ESP
#define CONFIG_TEMP_BOARD 1           // D1 R1


#define ACTIVE_CONFIG CONFIG_4_RELAY_BOARD

#define WIFI_CHANNEL 2

#define WIFI_SSID_STALNET "Stalnet"
#define WIFI_SSID_STALNET2 "Stalnet2"

#define WIFI_BSSID_STALNET {0x9C, 0xC7, 0xA6, 0x64, 0x07, 0x61}
#define WIFI_BSSID_STALNET2 {0x9C, 0xC7, 0xA6, 0x71, 0x1F, 0x01}

#if ACTIVE_CONFIG == CONFIG_4_RELAY_BOARD
  #warning Relay Board Configuration
  #define RELAY_COUNT 4
  #define TEMP_COUNT 0
  #define PIN_DIGITAL_TEMP_SENSORS 0
  #define DEVICE_NAME DEV_ID_4RELAY_BOARD
  #define TEMP_OFFSET 0
  #define TEMP_ADDRESS_MAPPING_0 {}
  #define TEMP_ADDRESS_MAPPING_1 {}
  #define TEMP_ADDRESS_MAPPING_2 {}
  #define TEMP_ADDRESS_MAPPING_3 {}
  #define WIFI_SSID WIFI_SSID_STALNET
  #define WIFI_BSSID WIFI_BSSID_STALNET
  #define WIFI_OTA_SUPPORT true
#elif ACTIVE_CONFIG == CONFIG_TEMP_BOARD
  #warning Temp Board Configuration
  #define RELAY_COUNT 0
  #define TEMP_COUNT 4
  #define PIN_DIGITAL_TEMP_SENSORS D3
  #define DEVICE_NAME DEV_ID_WATER_TEMP_BOARD
  #define TEMP_OFFSET 0
  #define TEMP_ADDRESS_MAPPING_0 {0x28,0xff,0x4c,0x01,0x67,0x14,0x02,0xbb}
  #define TEMP_ADDRESS_MAPPING_1 {0x28,0xff,0xbf,0x12,0x67,0x14,0x02,0xe3}
  #define TEMP_ADDRESS_MAPPING_2 {0x28,0xff,0x9a,0x13,0x67,0x14,0x02,0x46}
  #define TEMP_ADDRESS_MAPPING_3 {0x28,0xff,0xb3,0x8a,0x60,0x17,0x04,0xcd}

/*
  #define TEMP_ADDRESS_MAPPING_0 {0x28,0xff,0xb1,0x8c,0x60,0x17,0x04,0x47}
  #define TEMP_ADDRESS_MAPPING_1 {0x28,0xff,0xeb,0x8d,0x60,0x17,0x04,0xe0}
  #define TEMP_ADDRESS_MAPPING_2 {0x28,0xff,0x27,0xf6,0x66,0x14,0x02,0x86}
  #define TEMP_ADDRESS_MAPPING_3 {0x28,0xff,0x4d,0xbb,0x66,0x14,0x03,0x09}
  */
  
  #define WIFI_SSID WIFI_SSID_STALNET
  #define WIFI_BSSID WIFI_BSSID_STALNET
  #define WIFI_OTA_SUPPORT true
#endif

#if RELAY_COUNT == 0 && TEMP_COUNT == 0
  #error No valid configuration
#endif
