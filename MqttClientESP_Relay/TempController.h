#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractIntervalTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include "MqttController.h"
#include "TempAdapterDHT.h"
#include <FloatProperty.h>

#define TEMP_INTERVAL_MS 2000

#define TEMP_BC_INTERVAL_BC LIFETIME_MID/2

class TempController : public AbstractIntervalTask, public MqttController::MqttEventCallbackHandler, public FloatProperty::ValueChangeListener {
public:
  TempController();
  ~TempController();

  void init();

  void update();
  
  void onConnected();

  void initMapping();

  void onPropertyValueChange(uint8_t id, float newValue, float oldValue);

  void onBroadcast();
  String getBroadcastPath() {
    return MQTT_PATH_TEMPS;
  }
  int getID() {
    return TEMP_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

  void sendTemp(uint8_t index);

private:
  float getTemperature(uint8_t index);
  bool mappingInitialized = false;

  uint64_t lastBroadcast = 0;

  uint8_t currentTempIndexToUpdate = 0;

  TempAdapterDHT tempAdapter;
  bool hasValidMapping[TEMP_COUNT];
};


#endif
