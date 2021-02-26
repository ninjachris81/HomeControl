#ifndef DOORBELLCONTROLLER_H
#define DOORBELLCONTROLLER_H

#include <AbstractIdleTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include "MqttController.h"
#include "DFRobotDFPlayerMini.h"
#include <HardwareSerial.h>

#define LAST_PLAY_TIMEOUT_MS 4000

class DoorBellController : public AbstractIdleTask, public MqttController::MqttEventCallbackHandler {
public:
  DoorBellController();
  ~DoorBellController();

  void init();

  void onConnected();

  void onBroadcast();

  void setVolume(uint8_t volume);
  
  String getBroadcastPath() {
    return MQTT_PATH_EVENTS;
  }
  
  int getID() {
    return DOOR_BELL_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

private:
  uint64_t lastMqttUpdate = 0;
  uint64_t lastPlayStarted = 0;
  uint8_t currentVolume = 25;

  DFRobotDFPlayerMini myDFPlayer;

};


#endif
