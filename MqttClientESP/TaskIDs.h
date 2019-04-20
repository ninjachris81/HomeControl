#include "ESPConfigurations.h"

#define MQTT_CONTROLLER 0

#if RELAY_COUNT > 0
  #define RELAY_CONTROLLER 1
  #warning Using relay controller as 1
#else
  #define RELAY_CONTROLLER -1
#endif

#if TEMP_COUNT > 0
  #if RELAY_COUNT > 0
    #define TEMP_CONTROLLER 2
    #warning Using temp controller as 2
  #else
    #define TEMP_CONTROLLER 1
    #warning Using temp controller as 1
  #endif
#else
  #define TEMP_CONTROLLER -1
#endif
