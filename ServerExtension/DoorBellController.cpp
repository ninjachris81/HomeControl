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

  if (sensorState != 0) {
    sendValue = DOOR_STATE_TRUE;
  }

  if (millis() - lastSend > DOOR_SEND_INTERVAL_MS) {
    sendState();
    lastSend = millis();
    sendValue = DOOR_STATE_FALSE;
  }
}

void DoorBellController::sendState() {
  LOG_PRINT(F("BS:"));
  LOG_PRINTLN(sendValue);
}
