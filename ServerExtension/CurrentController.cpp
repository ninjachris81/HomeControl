#include "CurrentController.h"
#include <LogHelper.h>
#include "constants.h"

#define currCalibration 73.0

CurrentController::CurrentController() : AbstractIntervalTask(CURRENT_INTERVAL_MS) {
  
}

CurrentController::~CurrentController() {
}

void CurrentController::init() {
  //pinMode(PIN_EMON, INPUT);

  //adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  //analogReadResolution(ADC_BITS);

  emon.current(PIN_EMON, currCalibration);

  smoothCurrent.init(20, -999.0);
}

void CurrentController::update() {
  double amps = emon.calcIrms(1480);

  smoothCurrent.pushValue(amps);
  
  if (lastMqttUpdate==0 || millis() - lastMqttUpdate > MQTT_UPDATE_INTERVAL_MS) {
    lastMqttUpdate = millis();
    sendCurrent();
  }
}

void CurrentController::sendCurrent() {
  float c = smoothCurrent.getValue() - CURRENT_OFFSET;

  LOG_PRINT(F("BC:"));
  LOG_PRINTLN(c);
}
