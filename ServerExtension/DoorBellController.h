#ifndef DOORBELLCONTROLLER_H
#define DOORBELLCONTROLLER_H

#include <AbstractTask.h>

#include "TaskIDs.h"
#include "Pins.h"

#define DOOR_STATE_FALSE 0
#define DOOR_STATE_TRUE 1

#define DOOR_SEND_INTERVAL_MS 500

#define SENSOR_COUNT_THRESHOLD 600

class DoorBellController : public AbstractTask {
public:
  DoorBellController();
  ~DoorBellController();

  void init();

  void update();
  
private:
  void sendState(uint8_t state);
  int sensorCount = 0;

  unsigned long lastSend = 0;
  uint8_t sendValue = DOOR_STATE_FALSE;

};

#endif
