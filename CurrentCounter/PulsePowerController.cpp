#include "PulsePowerController.h"
#include <LogHelper.h>
#include "constants.h"
#include "DisplayController.h"

PulsePowerController::PulsePowerController() : AbstractTask() {
  
}

PulsePowerController::~PulsePowerController() {
}

void PulsePowerController::init() {
  pinMode(PIN_PULSE, INPUT);

  smoothPower.init(10, -999.0);
  smoothPeakCount.init(5, -999.0);

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);
}

void PulsePowerController::update() {
  adcRate++;
  
  bool isPeak = !digitalRead(PIN_PULSE);
  if (lastPeak!=isPeak) {
    if (!isPeak) {
      // changed, calculate on lower peak
      peakCount++;
    }
    
    lastPeak = isPeak;
  }

  if (lastCalculation==0 || millis() - lastCalculation >= PULSE_CALCULATION_INTERVAL_MS) {
    lastCalculation = millis();

    smoothPeakCount.pushValue(peakCount);
    smoothPower.pushValue(WATT_SEC_PER_PULSE * smoothPeakCount.getValue());
    totalPulseCount+=peakCount;
    taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setPower(smoothPower.getValue(), totalPulseCount, adcRate);
    
    peakCount = 0;
    adcRate = 0;
  }

  if (lastMqttUpdate==0 || millis() - lastMqttUpdate > MQTT_UPDATE_INTERVAL_MS) {
    lastMqttUpdate = millis();
    sendPower();
  }
}

void PulsePowerController::onConnected() {
  LOG_PRINTLN(F("MQTT connected"));
  onBroadcast();
  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setMqttConnected(true, taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->getIP());
}

void PulsePowerController::onBroadcast() {
  sendPower();
}

void PulsePowerController::onStringReceived(int index, String value) {
  // ignore
}

void PulsePowerController::onBoolReceived(int index, bool value) {
  // ignore
}

void PulsePowerController::onIntReceived(int index, int value) {
  // ignore
}

void PulsePowerController::onDoubleReceived(int index, double value) {
  // ignore
}

void PulsePowerController::sendPower() {
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_POWERS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_POWERS_MAIN)), smoothPower.getValue());
  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->increaseSentCount();
}
