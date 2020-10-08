// LOLIN ESP-32

#include "TaskManager.h"
#include "MqttController.h"
#include "CurrentController.h"
#include "DisplayController.h"
#include <LogHelper.h>

TaskManager taskManager;
MqttController mqttController(false, true);

CurrentController currentController;
DisplayController displayController;

void onConnectionEstablished() {}   // legacy

void setup() {
  LOG_INIT();

  taskManager.registerTask(&mqttController);
  
  taskManager.registerTask(&currentController);

  taskManager.registerTask(&displayController);

  taskManager.init();
  
  LOG_PRINTLN(F("INIT COMPLETE"));
}

void loop() {
  taskManager.update();
  delay(10);
}
