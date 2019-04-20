#include "TempAdapterDHT.h"
#include "Pins.h"

TempAdapterDHT::TempAdapterDHT() {
  oneWire = new OneWire(PIN_DIGITAL_TEMP_SENSORS);
  sensors = new DallasTemperature(oneWire);
}

void TempAdapterDHT::init() {
  sensors->begin();
  sensors->setWaitForConversion(false);

  byte addr[8];
  oneWire->search(addr);
  foundSensors = sensors->getDeviceCount();
}

double TempAdapterDHT::getTemperature(uint8_t index) {
  return temperatures[index].getValue();
}

void TempAdapterDHT::update() {
  bool hadError = false;

  if (foundSensors==TEMP_COUNT) {
    for (uint8_t i=0;i<TEMP_COUNT;i++) {
      float v = sensors->getTempCByIndex(i);
      if (v!=DEVICE_DISCONNECTED_C) {
        digitalTemps[i].pushValue(v);
        temperatures[i].setValue(digitalTemps[i].getValue());
      } else {
        hadError = true;
      }
    }
    sensors->requestTemperatures();
  } else {
    hadError = true;
  }
  
  if (hadError) {
    byte addr[8];
    
    if (oneWire->search(addr)) {
      foundSensors = sensors->getDeviceCount();
    } else {
      sensors->begin();
    }
  }
}
