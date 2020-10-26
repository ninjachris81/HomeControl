#ifndef IRCONTROLLER_H
#define IRCONTROLLER_H

#include <AbstractIntervalTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include "MqttController.h"

#define IR_INTERVAL_MS 100

#define MQTT_UPDATE_INTERVAL_MS 5000

class IRController : public AbstractIntervalTask, public MqttController::MqttEventCallbackHandler {
public:
  IRController();
  ~IRController();

  void init();

  void update();
  
  void onConnected();

  void onBroadcast();
  
  String getBroadcastPath() {
    return MQTT_PATH_POWERS;
  }
  
  int getID() {
    return IR_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

  void sendPower();

private:
  uint64_t lastMqttUpdate = 0;

};


#endif
