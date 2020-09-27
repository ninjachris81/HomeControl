#include "TempController.h"
#include <LogHelper.h>
#include "ESPConfigurations.h"

#include "TaskIDs.h"
#include "MqttController.h"


TempController::TempController() : AbstractIntervalTask(1000) {
#if TEMP_SENSOR == TEMP_SENSOR_DALLAS
  oneWire = new OneWire(PIN_DIGITAL_TEMP_SENSORS);
  sensors = new DallasTemperature(oneWire);
#elif TEMP_SENSOR == TEMP_SENSOR_LM75
#elif TEMP_SENSOR == TEMP_SENSOR_DHT22
  dht = new DHT(PIN_DIGITAL_TEMP_SENSORS, DHT22);
#endif
}

TempController::~TempController() {
}

void TempController::init() {
#if TEMP_SENSOR == TEMP_SENSOR_DALLAS
  sensors->begin();
  sensors->setWaitForConversion(true);
  sensors->setResolution(TEMPERATURE_RESOLUTION);
  sensors->requestTemperatures();
#elif TEMP_SENSOR == TEMP_SENSOR_LM75
  Wire.begin();
#elif TEMP_SENSOR == TEMP_SENSOR_DHT22
  dht->begin();
#endif
}

void TempController::update() {
  if (hasSent) return;
  
  if (taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) {
    // ok, send

    LOG_PRINTLN(F("Connected, reading temp"));
    
#if TEMP_SENSOR == TEMP_SENSOR_DALLAS
    float t = sensors->getTempCByIndex(0);

    if (t!=DEVICE_DISCONNECTED_C) {
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(TEMP_MQTT_PATH)), t);
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
#elif TEMP_SENSOR == TEMP_SENSOR_LM75
    float t = temperature.readTemperatureC();

    if (t!=-0.5) {
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(TEMP_MQTT_PATH)), t);
      hasSent = true;
    } else {
      if (retryCount<10) {
        retryCount++;
      } else {
        taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(F("Timeout reading from temp sensor"));
        hasSent = true;
      }
    }
#elif TEMP_SENSOR == TEMP_SENSOR_DHT22
  float t = dht->readTemperature();
  if (!isnan(t)) {
    LOG_PRINT(F("Temp: "));
    LOG_PRINTLN(t);
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(TEMP_MQTT_PATH)), t);
    hasSent = true;

    // next is optional
    float h = dht->readHumidity();
    if (!isnan(h)) {
      LOG_PRINT(F("Hum: "));
      LOG_PRINTLN(h);
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_HUMIDITIES + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_HUMIDITIES_OUTSIDE)), h);
    }
  } else {
    if (retryCount<10) {
      retryCount++;
    } else {
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(F("Timeout reading from temp sensor"));
      hasSent = true;
    }
  }

/*
 * TODO?
  float h = dht->readHumidity();
  if (!isnan(h)) {
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_TEMPS_INSIDE)), t);
  }
  */
#endif
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
