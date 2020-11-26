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

  temp.init(PROP_TEMP, -999.0, TEMP_CONTROLLER_BROADCAST_INTERVAL_MS);
  temp.registerValueChangeListener(this);
  
  humidity.init(PROP_HUMIDITY, -999.0, TEMP_CONTROLLER_BROADCAST_INTERVAL_MS);
  humidity.registerValueChangeListener(this);
}

void TempController::update() {
  float x = dht->readTemperature(false, true);

  // TEMP
  if (!isnan(x)) {
    temp.setValue(roundToOneDec(x), true);
  } else {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendError(F("Error while reading temp"));
    temp.disableBroadcast();
  }

  // HUM
  x = dht->readHumidity();
  if (!isnan(x)) {
    humidity.setValue(roundToOneDec(x), true);
  } else {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendError(F("Error while reading humidity"));
    humidity.disableBroadcast();
  }

  temp.update();
  humidity.update();
}

void TempController::onPropertyValueChange(uint8_t id, float newValue, float oldValue) {
  switch(id) {
    case PROP_TEMP:
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendTemp(newValue);
      break;
    case PROP_HUMIDITY:
      taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendHumidity(newValue);
      break;
  }
}

float TempController::roundToOneDec(float val) {
  long v = (long) (val * 10L);
  return (float) (v / 10.0);
}
