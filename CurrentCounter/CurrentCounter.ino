// WEMOS LOLIN32

//#define NO_LOG_SERIAL

#include "TaskManager.h"
#include "MqttController.h"
#include "CurrentController.h"
#include "DisplayController.h"
#include "PulsePowerController.h"
#include "IRController.h"
#include "ButtonController.h"
#include <LogHelper.h>

TaskManager taskManager;
MqttController mqttController(false, true);

CurrentController currentController;
DisplayController displayController;
PulsePowerController pulsePowerController;
IRController irController;
ButtonController buttonController;

void onConnectionEstablished() {}   // legacy

void setup() {
  LOG_INIT();

  taskManager.registerTask(&mqttController);
  
  taskManager.registerTask(&currentController);

  taskManager.registerTask(&displayController);

  taskManager.registerTask(&pulsePowerController);

  taskManager.registerTask(&irController);

  taskManager.registerTask(&buttonController);

  taskManager.init();
  
  LOG_PRINTLN(F("INIT COMPLETE"));
}

void loop() {
  taskManager.update();
  delay(1);
}
