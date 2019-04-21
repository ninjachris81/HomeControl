#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractIntervalTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include "MqttController.h"
#include "TempAdapterDHT.h"
#include <Property.h>

#define TEMP_INTERVAL_MS 1000

class TempController : public AbstractIntervalTask, public MqttController::MqttEventCallbackHandler, public Property<float>::ValueChangeListener {
public:
  TempController();
  ~TempController();

  void init();

  void update();
  
  void onConnected();

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
  bool isConnected = false;

  TempAdapterDHT tempAdapter;
};


#endif
