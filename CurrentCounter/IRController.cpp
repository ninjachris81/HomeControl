#include "IRController.h"
#include <LogHelper.h>
#include "constants.h"
#include "DisplayController.h"
#include <HardwareSerial.h>

IRController::IRController() : AbstractIntervalTask(IR_INTERVAL_MS) {
  
}

IRController::~IRController() {
}

void IRController::init() {
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);

  Serial1.begin(9600);
}

void IRController::update() {
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  while(Serial1.available()) {
    int dataByte = Serial1.read();
    LOG_PRINTLNF(dataByte, HEX);
  }

  if (lastMqttUpdate==0 || millis() - lastMqttUpdate > MQTT_UPDATE_INTERVAL_MS) {
    lastMqttUpdate = millis();
    sendPower();
  }
}

void IRController::onConnected() {
  LOG_PRINTLN(F("MQTT connected"));
  onBroadcast();
  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setMqttConnected(true, taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->getIP());
}

void IRController::onBroadcast() {
  sendPower();
}

void IRController::onStringReceived(int index, String value) {
  // ignore
}

void IRController::onBoolReceived(int index, bool value) {
  // ignore
}

void IRController::onIntReceived(int index, int value) {
  // ignore
}

void IRController::onDoubleReceived(int index, double value) {
  // ignore
}

void IRController::sendPower() {
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  //taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_CURRENTS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_CURRENTS_MAIN_BASEMENT)), smoothCurrent.getValue() * 1000);
  //taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->increaseSentCount();
}
