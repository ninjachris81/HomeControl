#include "DoorBellController.h"
#include <LogHelper.h>
#include "constants.h"


HardwareSerial hwSerial(1);

DoorBellController::DoorBellController() : AbstractIdleTask() {
  
}

DoorBellController::~DoorBellController() {
}

void DoorBellController::init() {
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);

  hwSerial.begin(9600, SERIAL_8N1, 18, 19);

  if (!myDFPlayer.begin(hwSerial)) {
    LOG_PRINT(F("Unable to begin DFPlayer"));
  } else {
    myDFPlayer.enableDAC();
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    myDFPlayer.setTimeOut(500);
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

    LOG_PRINT("MP3 Files:");
    LOG_PRINTLN(myDFPlayer.readFileCounts());
  }
}

void DoorBellController::onConnected() {
  LOG_PRINTLN(F("MQTT connected"));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(getID(), MQTT_PATH_EVENTS_DOOR_BELL, BUILD_PATH(MQTT_PATH_EVENTS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_EVENTS_DOOR_BELL)));
  myDFPlayer.volume(12);
  myDFPlayer.play(1);
}

void DoorBellController::setVolume(uint8_t volume) {
  currentVolume = constrain(volume, 0, 30);
  LOG_PRINT(F("Volume: "));
  LOG_PRINTLN(currentVolume);
}

void DoorBellController::onBroadcast() {
}

void DoorBellController::onStringReceived(int index, String value) {
  // ignore
}

void DoorBellController::onBoolReceived(int index, bool value) {
  // ignore
}

void DoorBellController::onIntReceived(int index, int value) {
  if (index==MQTT_PATH_EVENTS_DOOR_BELL) {
    LOG_PRINTLN(F("New door bell event"));
    if (lastPlayStarted==0 || millis() - lastPlayStarted > LAST_PLAY_TIMEOUT_MS) {
      lastPlayStarted = millis();
      myDFPlayer.volume(currentVolume);
      myDFPlayer.play(3);
    }
  }
}

void DoorBellController::onDoubleReceived(int index, double value) {
  // ignore
}
