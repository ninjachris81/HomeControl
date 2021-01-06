#ifndef PULSE_POWER_CONTROLLER_H
#define PULSE_POWER_CONTROLLER_H

#include <AbstractTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include <SmoothValue.h>

#define MQTT_UPDATE_INTERVAL_MS 5000

#define PULSE_CALCULATION_INTERVAL_MS 1000

#define PULSE_PER_KWH 10000
#define WATT_SEC_PER_5SEC PULSE_CALCULATION_INTERVAL_MS * 60 * 60

class PulsePowerController : public AbstractTask {
public:
  PulsePowerController();
  ~PulsePowerController();

  void init();

  void update();
  
  void sendPower();

private:
  uint64_t lastMqttUpdate = 0;
  bool lastPeak = false;
  
  double WATT_SEC_PER_PULSE = WATT_SEC_PER_5SEC / PULSE_PER_KWH;
  uint64_t lastCalculation = 0;
  uint16_t peakCount = 0;
  SmoothValue smoothPeakCount;
  uint16_t totalPulseCount = 0;
  uint16_t adcRate = 0;

  SmoothValue smoothPower;
};


#endif
