#include "RelayController.h"
#include "TaskIDs.h"
#include "TaskManager.h"

RelayController::RelayController() : AbstractIntervalTask(LIFETIME_MID/2) {
  
}

RelayController::~RelayController() {
}

void RelayController::init() {
  Serial.begin(RELAY_SERIAL_SPEED);
  
  for (uint8_t i=0;i<RELAY_COUNT;i++) {
    relayStates[i] = false;
  }

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);
}

void RelayController::update() {
  onBroadcast();
}

void RelayController::onConnected() {
  for (uint8_t i=0;i<RELAY_COUNT;i++) {
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(RELAY_CONTROLLER, i, BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_SET + String(MQTT_PATH_SEP) + i));
  }

  onBroadcast();
}

void RelayController::onBroadcast() {
  for (uint8_t i=0;i<RELAY_COUNT;i++) {
    sendState(i);
  }  
}

void RelayController::setState(uint8_t index, bool value) {
  byte relayData[4];
  
  relayStates[index] = value;
  
  relayData[0] = 0xA0;
  relayData[1] = index+1;
  relayData[2] = relayStates[index] ? 0x01 : 0x00;
  relayData[3] = relayData[0] + relayData[1] + relayData[2];
  
  Serial.write(relayData, sizeof(relayData));

  sendState(index);
}

void RelayController::onStringReceived(int index, String value) {
  // ignore
}

void RelayController::onBoolReceived(int index, bool value) {
  setState(index, value);
}

void RelayController::onIntReceived(int index, int value) {
  // ignore
}

void RelayController::onDoubleReceived(int index, double value) {
  // ignore
}

void RelayController::sendState(uint8_t i) {
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(i)), relayStates[i]);
}
