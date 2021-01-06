#include "PulsePowerController.h"
#include <LogHelper.h>
#include "constants.h"

PulsePowerController::PulsePowerController() : AbstractTask() {
  
}

PulsePowerController::~PulsePowerController() {
}

void PulsePowerController::init() {
  pinMode(PIN_PULSE, INPUT);

  smoothPower.init(10, -999.0);
  smoothPeakCount.init(5, -999.0);
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
    
    peakCount = 0;
    adcRate = 0;
  }

  if (lastMqttUpdate==0 || millis() - lastMqttUpdate > MQTT_UPDATE_INTERVAL_MS) {
    lastMqttUpdate = millis();
    sendPower();
  }
}

void PulsePowerController::sendPower() {
  LOG_PRINT(F("OP:"));
  LOG_PRINTLN(smoothPower.getValue());
}
