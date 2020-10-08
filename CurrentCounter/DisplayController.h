#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "OLEDDisplayUi.h"

#include <AbstractTask.h>

#include "TaskIDs.h"
#include "Pins.h"

class DisplayController : public AbstractTask {
public:
  DisplayController();
  ~DisplayController();

  void init();

  void update();

  void setMqttConnected(bool newValue, String newIP);

  void setCurrent(double newCurrent, double newCurrentSmooth);

  void increaseSentCount();

private:
  
};


#endif
