#ifndef CURRENTCONTROLLER_H
#define CURRENTCONTROLLER_H

#include <AbstractIntervalTask.h>

#define emonTxV3 1

#include "TaskIDs.h"
#include "Pins.h"
#include "MqttController.h"
#include "EmonLib.h"
#include <SmoothValue.h>

#define CURRENT_INTERVAL_MS 500

#define MQTT_UPDATE_INTERVAL_MS 5000

class CurrentController : public AbstractIntervalTask, public MqttController::MqttEventCallbackHandler {
public:
  CurrentController();
  ~CurrentController();

  void init();

  void update();
  
  void onConnected();

  void onBroadcast();
  
  String getBroadcastPath() {
    return MQTT_PATH_CURRENTS;
  }
  int getID() {
    return CURRENT_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

  void sendCurrent();

private:
  uint64_t lastMqttUpdate = 0;

  EnergyMonitor emon;

  SmoothValue smoothCurrent;
};


#endif
