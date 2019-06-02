#include "TempController.h"
#include <LogHelper.h>
#include "ESPConfigurations.h"

#include "TaskIDs.h"
#include "MqttController.h"


TempController::TempController() : AbstractTask() {
  oneWire = new OneWire(PIN_DIGITAL_TEMP_SENSORS);
  sensors = new DallasTemperature(oneWire);
}

TempController::~TempController() {
}

void TempController::init() {
  sensors->begin();
  sensors->setWaitForConversion(true);
  sensors->setResolution(TEMPERATURE_RESOLUTION);
  sensors->requestTemperatures();
}

void TempController::update() {
  if (hasSent) return;
  
  if (taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) {
    // ok, send
    float t = sensors->getTempCByIndex(0);

    if (t!=DEVICE_DISCONNECTED_C) {
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_TEMPS_INSIDE)), t);
      hasSent = true;
    } else {
      if (retryCount<10) {
        sensors->requestTemperatures();
        retryCount++;
      } else {
        taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(F("Timeout reading from temp sensor"));
        hasSent = true;
      }
    }
  }
}

bool TempController::isFinished() {
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
