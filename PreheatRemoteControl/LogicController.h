#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include <AbstractIntervalTask.h>
#include <Property.h>
#include "MqttController.h"

#define LOGIC_INTERVAL 10000

#define SUBSCRIBE_RELAYS_HC_PUMP 0
#define SUBSCRIBE_RELAYS_WATER_PUMP 1

#define SUBSCRIBE_TEMPS_HC 2
#define SUBSCRIBE_TEMPS_WATER 3
#define SUBSCRIBE_TEMPS_TANK 4

class LogicController : public AbstractIntervalTask, public Property<uint8_t>::ValueChangeListener, public MqttController::MqttEventCallbackHandler  {
public:
  LogicController();

  enum STATE {
    STATE_STARTING = 0,
    STATE_RUNNING,
    STATE_STOPPED
  };

  void onConnected();
  
  void onBroadcast();
  String getBroadcastPath();
  int getID();
  
  void onStringReceived(int index, String value);
  void onBoolReceived(int index, bool value);
  void onIntReceived(int index, int value);
  void onDoubleReceived(int index, double value);

  void init();

  void update();

  void onPropertyValueChange(uint8_t id, uint8_t newValue, uint8_t oldValue);

  STATE getState();

  void startHeating();

  void stopHeating();


private:
  Property<uint8_t> currentState;
  

};

#endif  // LOGICCONTROLLER_H
