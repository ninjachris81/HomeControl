#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <AbstractIdleTask.h>
#include "MqttController.h"
#include "TaskIDs.h"

#define EEPROM_ADDRESS 0

class SettingsController : public AbstractIdleTask, public MqttController::MqttEventCallbackHandler {
public:
  SettingsController();
  ~SettingsController();

  struct MySettings {
    uint8_t volume = 25;
  };

  void init();

  void onConnected();

  void onBroadcast();
  
  String getBroadcastPath() {
    return MQTT_PATH_SETTINGS;
  }
  
  int getID() {
    return SETTINGS_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

private:
  MySettings currentSettings;
};


#endif
