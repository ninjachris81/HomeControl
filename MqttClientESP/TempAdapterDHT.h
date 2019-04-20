#ifndef TEMPADAPTERDHT_H
#define TEMPADAPTERDHT_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SmoothValue.h>
#include <Property.h>
#include "ESPConfigurations.h"

class TempAdapterDHT {
  public:
    TempAdapterDHT();

    void init();

    void update();

    double getTemperature(uint8_t index);

private:
  uint8_t foundSensors = 0;
  OneWire *oneWire;
  DallasTemperature* sensors;
  
  SmoothValue digitalTemps[TEMP_COUNT];
  Property<float> temperatures[TEMP_COUNT];

};

#endif
