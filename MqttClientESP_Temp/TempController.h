#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractIntervalTask.h>
#include "ESPConfigurations.h"

#if TEMP_SENSOR == TEMP_SENSOR_DALLAS
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #warning DALLAS SENSOR
#elif TEMP_SENSOR == TEMP_SENSOR_LM75
  #include <Temperature_LM75_Derived.h>
  #warning LM75 SENSOR
#elif TEMP_SENSOR == TEMP_SENSOR_DHT22
  #include <DHT.h>
  #warning DHT22 SENSOR
#endif

#define TEMPERATURE_RESOLUTION 9

class TempController : public AbstractIntervalTask {
public:
  TempController();
  ~TempController();

  void init();

  void update();
  
  bool isFinished();
private:
#if TEMP_SENSOR == TEMP_SENSOR_DALLAS
  OneWire *oneWire;
  DallasTemperature* sensors;
#elif TEMP_SENSOR == TEMP_SENSOR_LM75
  Generic_LM75 temperature;
#elif TEMP_SENSOR == TEMP_SENSOR_DHT22
  DHT *dht;
#endif

  bool hasSent = false;
  uint8_t retryCount = 0;

};


#endif
