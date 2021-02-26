#include "PulsePowerController.h"
#include <LogHelper.h>
#include "constants.h"

volatile uint16_t peakCount = 0;
volatile uint64_t totalPulseCount = 0;

void increasePulseCount() {
  peakCount++;
  totalPulseCount++;
}

PulsePowerController::PulsePowerController() : AbstractIntervalTask(PULSE_CALCULATION_INTERVAL_MS) {
  
}

PulsePowerController::~PulsePowerController() {
}

void PulsePowerController::init() {
  pinMode(PIN_PULSE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PULSE), increasePulseCount, RISING);

  smoothPower.init(10, -999.0);
  smoothPeakCount.init(5, -999.0);

  /*
  Pulses / KHW  10000
  Pulse in KWH  0,00010
  Pulse in WH 0,1
  Pulse in WS 360
*/

  WATT_SEC_PER_PULSE = 360.0;      // WATT_SEC_PER_5SEC / PULSE_PER_KWH;
}

void PulsePowerController::update() {
  
  smoothPeakCount.pushValue(peakCount);
  peakCount = 0;
  smoothPower.pushValue(WATT_SEC_PER_PULSE * smoothPeakCount.getValue());

  if (lastMqttUpdate==0 || millis() - lastMqttUpdate > MQTT_UPDATE_INTERVAL_MS) {
    lastMqttUpdate = millis();
    sendPower();
  }
}

void PulsePowerController::sendPower() {
  LOG_PRINT(F("OP:"));
  LOG_PRINTLN(smoothPower.getValue());
}
