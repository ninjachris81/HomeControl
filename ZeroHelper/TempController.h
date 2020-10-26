#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractIntervalTask.h>
#include "Pins.h"
#include "DHT.h"
#include <Property.h>

#define TEMP_UPDATE_INTERVAL_MS 10000

#define PROP_TEMP 0
#define PROP_HUMIDITY 1

#define TEMP_CONTROLLER_BROADCAST_INTERVAL_MS 30000

class TempController : public AbstractIntervalTask, public Property<float>::ValueChangeListener {
public:
  TempController();
  ~TempController();

  void init();

  void update();

  void onPropertyValueChange(uint8_t id, float newValue, float oldValue);

  float roundToOneDec(float val);


private:
  uint64_t lastTemp = TEMP_CONTROLLER_BROADCAST_INTERVAL_MS;
  uint64_t lastHum = TEMP_CONTROLLER_BROADCAST_INTERVAL_MS;

  bool tempIsValid = false;
  bool humIsValid = false;

  DHT* dht;

  Property<float> temp;
  Property<float> humidity;

};


#endif
