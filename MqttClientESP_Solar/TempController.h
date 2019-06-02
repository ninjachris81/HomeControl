#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <AbstractTask.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_RESOLUTION 9

class TempController : public AbstractTask {
public:
  TempController();
  ~TempController();

  void init();

  void update();
  
  bool isFinished();
private:
  OneWire *oneWire;
  DallasTemperature* sensors;

  bool hasSent = false;
  uint8_t retryCount = 0;

};


#endif
