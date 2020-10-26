#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "OLEDDisplayUi.h"

#include <AbstractIntervalTask.h>

#include "TaskIDs.h"
#include "Pins.h"

#define DISPLAY_UPDATE_INTERVAL 500

class DisplayController : public AbstractIntervalTask {
public:
  DisplayController();
  ~DisplayController();

  void init();

  void update();

  void setMqttConnected(bool newValue, String newIP);

  void setCurrent(double newCurrent, double newCurrentSmooth);

  void setPower(double newPower, uint16_t newPulseCount, uint16_t newAdcRate);

  void increaseSentCount();

private:
  
};


#endif
