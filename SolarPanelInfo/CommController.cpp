#include "CommController.h"
#include <LogHelper.h>

CommController::CommController() : AbstractIntervalTask(UPDATE_INTERVAL_MS) {
  
}

void CommController::init() {
  LOG_INIT();
}

void CommController::update() {
  LOG_PRINT("{\"ts\":");
  LOG_PRINT(millis());
  LOG_PRINTLN("}");
}

void CommController::sendVoltage(uint16_t mamps) {
  LOG_PRINT("{\"mamps\":");
  LOG_PRINT(mamps);
  LOG_PRINTLN("}");
}

void CommController::sendTemp(uint8_t temp) {
  LOG_PRINT("{\"temp\":");
  LOG_PRINT(temp);
  LOG_PRINTLN("}");
}

void CommController::sendHumidity(uint8_t humidity) {
  LOG_PRINT("{\"hum\":");
  LOG_PRINT(humidity);
  LOG_PRINTLN("}");
}

void CommController::sendError(String error) {
  LOG_PRINT("{\"err\":\"");
  LOG_PRINT(error);
  LOG_PRINTLN("\"}");
}
