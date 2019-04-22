#include "TempController.h"
#include <LogHelper.h>

TempController::TempController() : AbstractIntervalTask(TEMP_INTERVAL_MS) {
  
}

TempController::~TempController() {
}

void TempController::init() {
  LOG_INIT();
  
  tempAdapter.init();
  tempAdapter.addListener(this);

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);
}

void TempController::update() {
  tempAdapter.update();
  if (tempAdapter.getFoundSensors()!=TEMP_COUNT) {
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError("Invalid sensors: " + tempAdapter.getFoundSensors());
  } else {
    if (lastBroadcast==0 || millis()>lastBroadcast+TEMP_BC_INTERVAL_BC) {
      onBroadcast();
      lastBroadcast = millis();
    }
  }
}

void TempController::onConnected() {
  isConnected = true;

  onBroadcast();
}

void TempController::onBroadcast() {
  for (uint8_t i=0;i<TEMP_COUNT;i++) {
    sendTemp(i);
  }
}

void TempController::onStringReceived(int index, String value) {
  // ignore
}

void TempController::onBoolReceived(int index, bool value) {
  // ignore
}

void TempController::onIntReceived(int index, int value) {
  // ignore
}

void TempController::onDoubleReceived(int index, double value) {
  // ignore
}

void TempController::onPropertyValueChange(uint8_t id, float newValue, float oldValue) {
  if (isConnected) {
    sendTemp(id);
  } else {
    // cannot send
  }
}

float TempController::getTemperature(uint8_t index) {
  return tempAdapter.getTemperature(index);
}

void TempController::sendTemp(uint8_t index) {
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(index)), getTemperature(index));
}
