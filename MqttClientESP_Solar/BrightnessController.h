#ifndef BRIGHTNESSCONTROLLER_H
#define BRIGHTNESSCONTROLLER_H

#include <AbstractTask.h>

#include "TaskIDs.h"
#include "MqttController.h"

class BrightnessController : public AbstractTask {
public:
  BrightnessController();
  ~BrightnessController();

  void init();

  void update();

  bool isFinished();
private:
  bool hasSent = false;
};


#endif    //BRIGHTNESSCONTROLLER_H
