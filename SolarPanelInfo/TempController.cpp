#include "TempController.h"
#include "TaskIDs.h"
#include <LogHelper.h>

#include "CommController.h"

TempController::TempController() : AbstractIntervalTask(UPDATE_INTERVAL_MS) {
}

TempController::~TempController() {
}

void TempController::init() {
  dht = new DHT(PIN_TEMP_SENSOR, DHT11);
  dht->begin();

  temp.init(PROP_TEMP, 0);
  temp.registerValueChangeListener(this);
  
  humidity.init(PROP_HUMIDITY, 0);
  humidity.registerValueChangeListener(this);
}

void TempController::update() {
  float x = dht->readTemperature(false, true);
  
  if (!isnan(x)) {
    temp.setValue(x);
  } else {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendError(F("Error while reading temp"));
  }

  x = dht->readHumidity();
  if (!isnan(x)) {
    humidity.setValue(x);
  } else {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendError(F("Error while reading humidity"));
  }
  
}

void TempController::onPropertyValueChange(uint8_t id, uint8_t newValue, uint8_t oldValue) {
  switch(id) {
    case PROP_TEMP:
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendTemp(newValue);
      break;
    case PROP_HUMIDITY:
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendHumidity(newValue);
      break;
  }
}
