#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractIntervalTask.h>
#include "Pins.h"
#include "DHT.h"
#include <BroadcastProperty.h>

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
  DHT* dht;

  BroadcastProperty<float> temp;
  BroadcastProperty<float> humidity;

};


#endif
