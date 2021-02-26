#include "SettingsController.h"
#include <LogHelper.h>
#include "constants.h"
#include "DoorBellController.h"
#include <EEPROM.h>

SettingsController::SettingsController() : AbstractIdleTask() {
  
}

SettingsController::~SettingsController() {
}

void SettingsController::init() {
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);

  EEPROM.begin(sizeof(MySettings));
  EEPROM.get(EEPROM_ADDRESS, currentSettings);
  LOG_PRINTLN(F("Settings:"));
  LOG_PRINTLN(currentSettings.volume);
  
  taskManager->getTask<DoorBellController*>(DOOR_BELL_CONTROLLER)->setVolume(currentSettings.volume);
}

void SettingsController::onConnected() {
  LOG_PRINTLN(F("MQTT connected"));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(getID(), MQTT_PATH_SETTINGS_DOOR_BELL_VOLUME, BUILD_PATH(MQTT_PATH_SETTINGS + String(MQTT_PATH_SEP) + MQTT_SET + String(MQTT_PATH_SEP) + String(MQTT_PATH_SETTINGS_DOOR_BELL_VOLUME)));
}

void SettingsController::onBroadcast() {
}

void SettingsController::onStringReceived(int index, String value) {
  // ignore
}

void SettingsController::onBoolReceived(int index, bool value) {
  // ignore
}

void SettingsController::onIntReceived(int index, int value) {
  if (index==MQTT_PATH_SETTINGS_DOOR_BELL_VOLUME) {
    LOG_PRINT(F("New door bell volume"));
    LOG_PRINTLN(value);
    
    uint8_t newVolume = constrain(value, 0, 30);
    taskManager->getTask<DoorBellController*>(DOOR_BELL_CONTROLLER)->setVolume(newVolume);
    currentSettings.volume = newVolume;
    EEPROM.put(EEPROM_ADDRESS, currentSettings);
    EEPROM.commit();
  }
}

void SettingsController::onDoubleReceived(int index, double value) {
  // ignore
}
