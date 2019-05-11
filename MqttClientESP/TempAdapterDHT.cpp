#include "TempAdapterDHT.h"
#include "Pins.h"
#include <LogHelper.h>

TempAdapterDHT::TempAdapterDHT() {
  oneWire = new OneWire(PIN_DIGITAL_TEMP_SENSORS);
  sensors = new DallasTemperature(oneWire);
}

void TempAdapterDHT::init() {
  for (uint8_t i=0;i<TEMP_COUNT;i++) {
    digitalTemps[i].init(10, 0);
    temperatures[i].init(i);
    temperatures[i].setTolerance(TEMPERATURE_TOLERANCE);
    memset(addressMappings[i], 0, 8);
  }
  
  sensors->begin();
  sensors->setWaitForConversion(false);
  sensors->setResolution(TEMPERATURE_RESOLUTION);
  sensors->requestTemperatures();

  foundSensors = sensors->getDeviceCount();
}

void TempAdapterDHT::setMapping(uint8_t index, uint8_t *addr) {
  memcpy(addressMappings[index], addr, 8);
}

bool TempAdapterDHT::checkMapping(uint8_t index) {
  uint8_t *addr = addressMappings[index];
  
  if (sensors->isConnected(addr)) {
    LOG_PRINT(F("Sensor is connected: "));
    LOG_PRINTLN(index);
    return true;
  } else {
    return false;
  }
}

bool TempAdapterDHT::getUnmappedDevices(String &list) {
  foundSensors = sensors->getDeviceCount();

  for (uint8_t i=0;i<foundSensors;i++) {
    uint8_t addr[8];
    if (sensors->getAddress(addr, i)) {

      String sensorFoundAddr = addressToString(addr);
      bool found = false;
      
      for (uint8_t o=0;o<TEMP_COUNT;o++) {
        String mappedAddr = addressToString(addressMappings[o]);
        if (sensorFoundAddr==mappedAddr) {
          found = true;
          break;
        }
      }

      if (!found) {
        list+=sensorFoundAddr + " ";
      }
    } else {
      LOG_PRINTLN("Error while reading addr");
    }
  }

  list.trim();
  return list=="";
}

String TempAdapterDHT::addressToString(uint8_t *addr) {
  String returnStr;

  for (uint8_t i=0;i<8;i++) {
    String tmp = String(addr[i], HEX);
    if (tmp.length()==1) tmp = "0" + tmp;
    returnStr+="0x" + tmp;
    if (i<7) returnStr+=",";
  }
  
  //returnStr.remove((8*5) - 1,1);
  
  return returnStr;
}

float TempAdapterDHT::getTemperature(uint8_t index) {
  return temperatures[index].getValue();
}

void TempAdapterDHT::addListener(FloatProperty::ValueChangeListener *listener) {
  for (uint8_t i=0;i<TEMP_COUNT;i++) temperatures[i].registerValueChangeListener(listener);
}

uint8_t TempAdapterDHT::getFoundSensors() {
  return foundSensors;
}

bool TempAdapterDHT::sensorsValid() {
  return foundSensors==TEMP_COUNT;
}

void TempAdapterDHT::updateTemperature(uint8_t index) {
  float v = sensors->getTempC(addressMappings[index]);
  if (v!=DEVICE_DISCONNECTED_C) {
    digitalTemps[index].pushValue(v);
    temperatures[index].setValue(digitalTemps[index].getValue());
  } else {
    // TODO
  }
}

void TempAdapterDHT::update() {
  sensors->requestTemperatures();

  foundSensors = sensors->getDeviceCount();
}
