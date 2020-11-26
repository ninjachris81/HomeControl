#ifndef PULSE_POWER_CONTROLLER_H
#define PULSE_POWER_CONTROLLER_H

#include <AbstractTask.h>

#include "TaskIDs.h"
#include "Pins.h"
#include "MqttController.h"
#include <SmoothValue.h>

#define MQTT_UPDATE_INTERVAL_MS 5000

#define PULSE_CALCULATION_INTERVAL_MS 1000

#define PULSE_PER_KWH 10000
#define WATT_SEC_PER_5SEC PULSE_CALCULATION_INTERVAL_MS * 60 * 60

class PulsePowerController : public AbstractTask, public MqttController::MqttEventCallbackHandler {
public:
  PulsePowerController();
  ~PulsePowerController();

  void init();

  void update();
  
  void onConnected();

  void onBroadcast();
  
  String getBroadcastPath() {
    return MQTT_PATH_CURRENTS;
  }
  int getID() {
    return PULSE_POWER_CONTROLLER;
  }
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

  void sendPower();

private:
  uint64_t lastMqttUpdate = 0;
  bool lastPeak = false;
  
  double WATT_SEC_PER_PULSE = WATT_SEC_PER_5SEC / PULSE_PER_KWH;
  uint64_t lastCalculation = 0;
  uint16_t peakCount = 0;
  SmoothValue smoothPeakCount;
  uint16_t totalPulseCount = 0;
  uint16_t adcRate = 0;

  SmoothValue smoothPower;
};


#endif
