#include "BrightnessController.h"
#include "ESPConfigurations.h"
#include <LogHelper.h>

BrightnessController::BrightnessController() : AbstractTask() {
  
}

BrightnessController::~BrightnessController() {
}

void BrightnessController::init() {
  pinMode(PIN_BRIGHTNESS, INPUT);
}

void BrightnessController::update() {
  if (hasSent) return;
  
  if (taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) {
    // ok, send
    int brightness = analogRead(PIN_BRIGHTNESS);
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_BRIGHTNESSES + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_BRIGHTNESSES_SOLAR)), brightness);
    hasSent = true;
  }
}

bool BrightnessController::isFinished() {
  return hasSent;
}

/*
void TempController::sendTemp(uint8_t index) {
  if (tempAdapter.sensorsValid() && hasValidMapping[index]) {
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(TEMP_OFFSET + index)), getTemperature(index));
  } else {
    LOG_PRINTLN(F("Invalid sensors or mapping"));
  }
}*/
