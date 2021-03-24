// Arduino Nano

#include "TaskManager.h"
#include "CommController.h"
#include "CurrentController.h"

CommController commController;
CurrentController currentController;

TaskManager taskManager;

void setup() {
  taskManager.registerTask(&commController);
  taskManager.registerTask(&currentController);

  taskManager.init();
}
 
void loop() {
  taskManager.update();
  delay(10);
}
