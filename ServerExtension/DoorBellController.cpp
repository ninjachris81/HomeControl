#include "DoorBellController.h"
#include <LogHelper.h>

DoorBellController::DoorBellController() : AbstractTask() {
  
}

DoorBellController::~DoorBellController() {
}

void DoorBellController::init() {
  pinMode(PIN_DOOR_BELL, INPUT);
}

void DoorBellController::update() {
  int sensorState = analogRead(PIN_DOOR_BELL);

  sensorCount+=sensorState;

  if (sensorState != 0 && sensorCount > SENSOR_COUNT_THRESHOLD) {
    sendValue = DOOR_STATE_TRUE;
  }

  if (millis() - lastSend > DOOR_SEND_INTERVAL_MS) {
    sendState(sendValue);
    
    // reset
    lastSend = millis();
    sendValue = DOOR_STATE_FALSE;
    sensorCount = 0;
  }
}

void DoorBellController::sendState(uint8_t state) {
  LOG_PRINT(F("BS:"));
  LOG_PRINTLN(state);
}
