#include "TaskManager.h"
#include "CommController.h"
#include "VoltageController.h"
#include "TempController.h"

CommController commController;
VoltageController voltageController;
TempController tempController;

TaskManager taskManager;

void setup() {
  taskManager.registerTask(&commController);
  taskManager.registerTask(&voltageController);
  taskManager.registerTask(&tempController);

  taskManager.init();
}
 
void loop() {
  taskManager.update();
  delay(10);
}
