#include "TempController.h"

TempController::TempController() : AbstractIntervalTask(TEMP_INTERVAL_MS) {
  
}

TempController::~TempController() {
}

void TempController::init() {
  tempAdapter.init();
}

void TempController::update() {
  tempAdapter.update();
}

void TempController::onConnected() {
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

double TempController::getTemperature(uint8_t index) {
  
}

void TempController::sendTemp(uint8_t index) {
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(index)), getTemperature(index));
}
