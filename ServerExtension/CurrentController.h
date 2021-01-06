#ifndef CURRENTCONTROLLER_H
#define CURRENTCONTROLLER_H

#include <AbstractIntervalTask.h>

#define emonTxV3 1

#include "TaskIDs.h"
#include "Pins.h"
#include "EmonLib.h"
#include <SmoothValue.h>

#define CURRENT_INTERVAL_MS 500

#define MQTT_UPDATE_INTERVAL_MS 5000

#define CURRENT_OFFSET 0.18

class CurrentController : public AbstractIntervalTask {
public:
  CurrentController();
  ~CurrentController();

  void init();

  void update();
  
  void sendCurrent();

private:
  uint64_t lastMqttUpdate = 0;

  EnergyMonitor emon;

  SmoothValue smoothCurrent;
};


#endif
