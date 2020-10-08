#include "CurrentController.h"
#include <LogHelper.h>
#include "constants.h"
#include "DisplayController.h"
#include <driver/adc.h>

#define currCalibration 15

CurrentController::CurrentController() : AbstractIntervalTask(CURRENT_INTERVAL_MS) {
  
}

CurrentController::~CurrentController() {
}

void CurrentController::init() {
  //pinMode(PIN_EMON, INPUT);

  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);

  emon.current(PIN_EMON, currCalibration);

  smoothCurrent.init(20, 0.0);
  
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);
}

void CurrentController::update() {
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  double amps = emon.calcIrms(1480);

  LOG_PRINTLN(amps);

  smoothCurrent.pushValue(amps);
  
  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setCurrent(amps, smoothCurrent.getValue());

  if (lastMqttUpdate==0 || millis() - lastMqttUpdate > MQTT_UPDATE_INTERVAL_MS) {
    lastMqttUpdate = millis();
    sendCurrent();
  }
}

void CurrentController::onConnected() {
  LOG_PRINTLN(F("MQTT connected"));
  onBroadcast();
  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setMqttConnected(true, taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->getIP());
}

void CurrentController::onBroadcast() {
  sendCurrent();
}

void CurrentController::onStringReceived(int index, String value) {
  // ignore
}

void CurrentController::onBoolReceived(int index, bool value) {
  // ignore
}

void CurrentController::onIntReceived(int index, int value) {
  // ignore
}

void CurrentController::onDoubleReceived(int index, double value) {
  // ignore
}

void CurrentController::sendCurrent() {
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_CURRENTS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_CURRENTS_MAIN_BASEMENT)), smoothCurrent.getValue() * 1000);
  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->increaseSentCount();
}
