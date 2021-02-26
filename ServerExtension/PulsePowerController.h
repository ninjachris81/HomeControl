#ifndef PULSE_POWER_CONTROLLER_H
#define PULSE_POWER_CONTROLLER_H

#include <AbstractIntervalTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include <SmoothValue.h>

#define MQTT_UPDATE_INTERVAL_MS 5000

#define PULSE_CALCULATION_INTERVAL_MS 1000

#define PULSE_PER_KWH 10000
#define WATT_SEC_PER_5SEC PULSE_CALCULATION_INTERVAL_MS * 60 * 60 

class PulsePowerController : public AbstractIntervalTask {
public:
  PulsePowerController();
  ~PulsePowerController();

  void init();

  void update();
  
  void sendPower();

private:
  uint64_t lastMqttUpdate = 0;
  
  double WATT_SEC_PER_PULSE;
  SmoothValue smoothPeakCount;

  SmoothValue smoothPower;
};


#endif
