#include <LogHelper.h>
#include <TaskManager.h>

#include "MqttController.h"
#include "DisplayController.h"
#include "ButtonController.h"
#include "LogicController.h"

TaskManager taskManager;

MqttController mqttController;
DisplayController displayController;
ButtonController buttonController;
LogicController logicController;

void setup() {
  LOG_INIT();
  LOG_WAIT();

  taskManager.registerTask(&mqttController);
  taskManager.registerTask(&displayController);  
  taskManager.registerTask(&buttonController);
  taskManager.registerTask(&logicController);
  
  taskManager.init();

  LOG_PRINTLN(F("Init complete"));
}

void loop() {
  taskManager.update();
  delay(10);
}
