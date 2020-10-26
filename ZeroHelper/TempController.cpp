#include "TempController.h"
#include "TaskIDs.h"
#include <LogHelper.h>

#include "CommController.h"

TempController::TempController() : AbstractIntervalTask(TEMP_UPDATE_INTERVAL_MS) {
}

TempController::~TempController() {
}

void TempController::init() {
  dht = new DHT(PIN_TEMP_SENSOR, DHT22);
  dht->begin();

  temp.init(PROP_TEMP, -999.0);
  temp.registerValueChangeListener(this);
  
  humidity.init(PROP_HUMIDITY, -999.0);
  humidity.registerValueChangeListener(this);
}

void TempController::update() {
  float x = dht->readTemperature(false, true);

  // TEMP
  if (!isnan(x)) {
    temp.setValue(roundToOneDec(x));
    tempIsValid = true;
  } else {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendError(F("Error while reading temp"));
    tempIsValid = false;
  }

  // HUM
  x = dht->readHumidity();
  if (!isnan(x)) {
    humidity.setValue(roundToOneDec(x));
    humIsValid = true;
  } else {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendError(F("Error while reading humidity"));
    humIsValid = false;
  }

  // check bc
  if (tempIsValid && millis() - lastTemp > TEMP_CONTROLLER_BROADCAST_INTERVAL_MS) {
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendTemp(temp.getValue());
      lastTemp = millis();
  }

  if (humIsValid && millis() - lastHum > TEMP_CONTROLLER_BROADCAST_INTERVAL_MS) {
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendHumidity(humidity.getValue());
      lastHum = millis();
  }
}

void TempController::onPropertyValueChange(uint8_t id, float newValue, float oldValue) {
  switch(id) {
    case PROP_TEMP:
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendTemp(newValue);
      lastTemp = millis();
      break;
    case PROP_HUMIDITY:
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendHumidity(newValue);
      lastHum = millis();
      break;
  }
}

float TempController::roundToOneDec(float val) {
  long v = (long) (val * 10L);
  return (float) (v / 10.0);
}
