#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractIntervalTask.h>
#include "Pins.h"
#include "DHT.h"
#include <Property.h>

#define UPDATE_INTERVAL_MS 10000

#define PROP_TEMP 0
#define PROP_HUMIDITY 1

class TempController : public AbstractIntervalTask, public Property<uint8_t>::ValueChangeListener {
public:
  TempController();
  ~TempController();

  void init();

  void update();

  void onPropertyValueChange(uint8_t id, uint8_t newValue, uint8_t oldValue);


private:
  DHT* dht;

  Property<uint8_t> temp;
  Property<uint8_t> humidity;

};


#endif
