#ifndef TEMPADAPTERDHT_H
#define TEMPADAPTERDHT_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SmoothValue.h>
#include <Property.h>
#include "ESPConfigurations.h"

#define TEMPERATURE_PRECISION 9

class TempAdapterDHT {
  public:
    TempAdapterDHT();

    void init();

    void update();

    void updateTemperature(uint8_t index);

    float getTemperature(uint8_t index);

    void setMapping(uint8_t index, uint8_t *addr);

    bool checkMapping(uint8_t index);

    bool getUnmappedDevices(String &list);

    void addListener(Property<float>::ValueChangeListener *listener);

    uint8_t getFoundSensors();

    bool sensorsValid();

    static String addressToString(uint8_t *addr);

private:
  uint8_t foundSensors = 0;
  OneWire *oneWire;
  DallasTemperature* sensors;
  
  SmoothValue digitalTemps[TEMP_COUNT];
  Property<float> temperatures[TEMP_COUNT];
  uint8_t addressMappings[TEMP_COUNT][8];

};

#endif
