// Nano

//#define NO_LOG_SERIAL

#include "TaskManager.h"
#include "CurrentController.h"
#include "PulsePowerController.h"
#include "DoorBellController.h"
#include <LogHelper.h>

TaskManager taskManager;
CurrentController currentController;
PulsePowerController pulsePowerController;
DoorBellController doorBellController;

void setup() {
  LOG_INIT();

  taskManager.registerTask(&currentController);

  taskManager.registerTask(&pulsePowerController);

  taskManager.registerTask(&doorBellController);

  taskManager.init();
  
  LOG_PRINTLN(F("INIT COMPLETE"));
}

void loop() {
  taskManager.update();
  delay(1);
}
