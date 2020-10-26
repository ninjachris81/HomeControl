#ifndef VOLTAGECONTROLLER_H
#define VOLTAGECONTROLLER_H

#include <AbstractIntervalTask.h>
#include <Property.h>

#define CURRENT_UPDATE_INTERVAL_MS 5000

#define PROP_MAMPS 0

#define MIN_VALUE 0
#define MAX_VALUE 1500

#define CURRENT_CONTROLLER_BROADCAST_INTERVAL_MS 30000

class CurrentController : public AbstractIntervalTask, public Property<uint16_t>::ValueChangeListener {
public:
  CurrentController();

  void init();

  void update();

  void onPropertyValueChange(uint8_t id, uint16_t newValue, uint16_t oldValue);

private:
  uint64_t lastCurr = CURRENT_CONTROLLER_BROADCAST_INTERVAL_MS;
  bool currIsValid = false;

  Property<uint16_t> mamps;
  float getVPP();
};


#endif    //VOLTAGECONTROLLER_H
