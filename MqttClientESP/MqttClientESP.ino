#include "ESPConfigurations.h"

#include "constants.h"

#include "TaskManager.h"
#include "MqttController.h"
#include "RelayController.h"
#include "TempController.h"

TaskManager taskManager;
MqttController mqttController;

#if RELAY_COUNT > 0
  RelayController relayController;
#endif

#if TEMP_COUNT > 0
  TempController tempController;
#endif

void onConnectionEstablished() {}   // legacy

void setup () {
  taskManager.registerTask(&mqttController);
#if RELAY_COUNT > 0
  taskManager.registerTask(&relayController);
#endif
#if TEMP_COUNT > 0
  taskManager.registerTask(&tempController);
#endif

  taskManager.init();
}

void loop()
{
  taskManager.update();
  delay(10);
}
