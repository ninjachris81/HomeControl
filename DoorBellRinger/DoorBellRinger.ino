// NodeMCU-32S

//#define NO_LOG_SERIAL

#include "TaskManager.h"
#include "MqttController.h"
#include "DoorBellController.h"
#include <LogHelper.h>

TaskManager taskManager;
MqttController mqttController(false, true);
DoorBellController doorBellController;

void onConnectionEstablished() {}   // legacy

void setup() {
  
  LOG_INIT();

  taskManager.registerTask(&mqttController);
  
  taskManager.registerTask(&doorBellController);

  taskManager.init();
  
  LOG_PRINTLN(F("INIT COMPLETE"));
}

void loop() {
  taskManager.update();
  delay(1);
}
