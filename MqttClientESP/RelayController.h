#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <AbstractIntervalTask.h>
#include "MqttController.h"
#include "constants.h"
#include "TaskIDs.h"
#include "ESPConfigurations.h"

#define RELAY_SERIAL_SPEED 115200

class RelayController : public AbstractIntervalTask, public MqttController::MqttEventCallbackHandler {
public:
  RelayController();
  ~RelayController();
  
  void init();

  void update();

  void onConnected();

  void onBroadcast();
  String getBroadcastPath() {
    return MQTT_PATH_RELAYS;
  }
  int getID() {
    return RELAY_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);


  void sendState(uint8_t index);

  void setState(uint8_t index, bool value);

private:
  bool relayStates[RELAY_COUNT];
};


#endif
