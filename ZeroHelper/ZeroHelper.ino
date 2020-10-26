// Arduino Nano

#include "TaskManager.h"
#include "CommController.h"
#include "CurrentController.h"
#include "TempController.h"

CommController commController;
CurrentController currentController;
TempController tempController;

TaskManager taskManager;

void setup() {
  taskManager.registerTask(&commController);
  taskManager.registerTask(&currentController);
  taskManager.registerTask(&tempController);

  taskManager.init();
}
 
void loop() {
  taskManager.update();
  delay(10);
}
