#include "LogicController.h"

#include <LogHelper.h>
#include "TaskIDs.h"
#include "DisplayController.h"

LogicController::LogicController() : AbstractIntervalTask(LOGIC_INTERVAL) {
}

void LogicController::init() {
  currentState.init(0, STATE_STARTING);
  currentState.registerValueChangeListener(this);

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);
}

void LogicController::update() {
  bool wifiConnected = taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected();
  
  if (wifiConnected) {
    currentState.setValue(STATE_STOPPED);
  } else {
    currentState.setValue(STATE_STARTING);
  }
}



void LogicController::onConnected() {
  LOG_PRINTLN(F("onConnected"));
  
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(LOGIC_CONTROLLER, SUBSCRIBE_RELAYS_HC_PUMP, BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_RELAYS_HC_PUMP)));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(LOGIC_CONTROLLER, SUBSCRIBE_RELAYS_WATER_PUMP, BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_RELAYS_WATER_PUMP)));

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(LOGIC_CONTROLLER, SUBSCRIBE_TEMPS_HC, BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_TEMPS_HC)));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(LOGIC_CONTROLLER, SUBSCRIBE_TEMPS_WATER, BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_TEMPS_WATER)));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->subscribePath(LOGIC_CONTROLLER, SUBSCRIBE_TEMPS_TANK, BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_TEMPS_TANK)));

  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setValue(DisplayController::PROP_IS_CONNECTED, true);
}

void LogicController::onBroadcast() {
  
}

String LogicController::getBroadcastPath() {
  return "";
}

int LogicController::getID() {
  return LOGIC_CONTROLLER;
}

void LogicController::onStringReceived(int index, String value) {
  // ignore
}

void LogicController::onBoolReceived(int index, bool value) {
  LOG_PRINT(F("Bool received "));
  LOG_PRINTLN(value);
  
  switch (index) {
    case SUBSCRIBE_RELAYS_HC_PUMP:
      // MQTT_PATH_RELAYS_HC_PUMP state changed
      if (value) {
        currentState.setValue(STATE_RUNNING);
      } else {
        currentState.setValue(STATE_STOPPED);
      }
      taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setValue(DisplayController::PROP_PUMP_WATER, value?true:false);
      break;
    case SUBSCRIBE_RELAYS_WATER_PUMP:
      // MQTT_PATH_RELAYS_HC_PUMP state changed
      if (value) {
        currentState.setValue(STATE_RUNNING);
      } else {
        currentState.setValue(STATE_STOPPED);
      }
      taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setValue(DisplayController::PROP_PUMP_HC, value?true:false);
      break;
  }
}

void LogicController::onIntReceived(int index, int value) {
  // ignore
}

void LogicController::onDoubleReceived(int index, double value) {
  LOG_PRINT(F("Double received "));
  LOG_PRINTLN(value);

  switch(index) {
    case SUBSCRIBE_TEMPS_HC:
      taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setValue(DisplayController::PROP_TEMP_HC, value);
      break;
    case SUBSCRIBE_TEMPS_WATER:
      taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setValue(DisplayController::PROP_TEMP_WATER, value);
      break;
    case SUBSCRIBE_TEMPS_TANK:
      taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->setValue(DisplayController::PROP_TEMP_TANK, value);
      break;
  }
}





LogicController::STATE LogicController::getState() {
  return (STATE)currentState.getValue();
}

void LogicController::onPropertyValueChange(uint8_t id, uint8_t newValue, uint8_t oldValue) {
  LOG_PRINT(F("State: "));
  LOG_PRINTLN(newValue);

  switch(newValue) {
    case STATE_STARTING:
      break;
    case STATE_RUNNING:
      break;
    case STATE_STOPPED:
      break;
  }

  taskManager->getTask<DisplayController*>(DISPLAY_CONTROLLER)->triggerUpdate();
}

void LogicController::startHeating() {
  LOG_PRINTLN(F("Start heating"));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_CMD), MQTT_CMD_START_PREHEAT);
}

void LogicController::stopHeating() {
  LOG_PRINTLN(F("Stop heating"));
  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_CMD), MQTT_CMD_STOP_PREHEAT);
}
