#ifndef VOLTAGECONTROLLER_H
#define VOLTAGECONTROLLER_H

#include <AbstractIntervalTask.h>
#include <Property.h>

#define SEND_INTERVAL_MS 5000

#define PROP_MAMPS 0

#define MIN_VALUE 0
#define MAX_VALUE 1500

class VoltageController : public AbstractIntervalTask, public Property<uint16_t>::ValueChangeListener {
public:
  VoltageController();

  void init();

  void update();

  void onPropertyValueChange(uint8_t id, uint16_t newValue, uint16_t oldValue);

private:
  Property<uint16_t> mamps;
  float getVPP();
};


#endif    //VOLTAGECONTROLLER_H
