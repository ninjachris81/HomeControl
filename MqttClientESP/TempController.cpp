#include "TempController.h"
#include <LogHelper.h>

TempController::TempController() : AbstractIntervalTask(TEMP_INTERVAL_MS) {
  
}

TempController::~TempController() {
}

void TempController::init() {
  LOG_INIT();
  
  tempAdapter.init();
  tempAdapter.addListener(this);

  taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->registerHandler(this);

  for (uint8_t i=0;i<TEMP_COUNT;i++) hasValidMapping[i] = false;
}

void TempController::initMapping() {

  for (uint8_t i=0;i<TEMP_COUNT;i++) {
    uint8_t mapping[8];
    
    switch(i) {
      case 0: {
        byte tmp[8]=TEMP_ADDRESS_MAPPING_0;
        memcpy(mapping, tmp, 8);
        break;
      }
      case 1: {
        byte tmp[8]=TEMP_ADDRESS_MAPPING_1;
        memcpy(mapping, tmp, 8);
        break;
      }
      case 2: {
        byte tmp[8]=TEMP_ADDRESS_MAPPING_2;
        memcpy(mapping, tmp, 8);
        break;
      }
      case 3: {
        byte tmp[8]=TEMP_ADDRESS_MAPPING_3;
        memcpy(mapping, tmp, 8);
        break;
      }
      default:
        taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(F("Invalid mapping index:"), i);
        mappingInitialized = true;
        return;
        break;
    }
    
    tempAdapter.setMapping(i, mapping);
    
    if (!tempAdapter.checkMapping(i)) {
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(F("Invalid mapping for:"), i);
      hasValidMapping[i] = false;
    } else {
      hasValidMapping[i] = true;
    }
  }

  String unmappedList;
  if (!tempAdapter.getUnmappedDevices(unmappedList)) {

    LOG_PRINTLN(unmappedList);

    int i = unmappedList.indexOf(" ");
    while (i>=0) {
      String device = unmappedList.substring(0, i);
      unmappedList = unmappedList.substring(i+1);
      unmappedList.trim();
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(String("Unmapped device: ") + device);
      i = unmappedList.indexOf(" ");
    }

    if (unmappedList.length()>0) {
      taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(String("Unmapped device: ") + unmappedList);
    }
    
  }

  mappingInitialized = true;
}

void TempController::update() {
  if (!mappingInitialized) initMapping();
  
  tempAdapter.update();
  
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  if (!tempAdapter.sensorsValid()) {
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendError(F("Invalid sensors:"), tempAdapter.getFoundSensors());

    LOG_PRINT(F("Invalid sensors "));
    LOG_PRINTLN(tempAdapter.getFoundSensors());

    // TODO: reset ?
  } else {
    if (lastBroadcast==0 || millis()>lastBroadcast+TEMP_BC_INTERVAL_BC) {
      onBroadcast();
      lastBroadcast = millis();
    } else {
      for (uint8_t i=0;i<TEMP_COUNT;i++) {
        if (hasValidMapping[i]) {
          tempAdapter.updateTemperature(i);
        }
      }
    }
  }
}

void TempController::onConnected() {
  onBroadcast();
}

void TempController::onBroadcast() {
  for (uint8_t i=0;i<TEMP_COUNT;i++) {
    sendTemp(i);
  }
}

void TempController::onStringReceived(int index, String value) {
  // ignore
}

void TempController::onBoolReceived(int index, bool value) {
  // ignore
}

void TempController::onIntReceived(int index, int value) {
  // ignore
}

void TempController::onDoubleReceived(int index, double value) {
  // ignore
}

void TempController::onPropertyValueChange(uint8_t id, float newValue, float oldValue) {
  sendTemp(id);
}

float TempController::getTemperature(uint8_t index) {
  return tempAdapter.getTemperature(index);
}

void TempController::sendTemp(uint8_t index) {
  if (!taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) return;

  if (tempAdapter.sensorsValid() && hasValidMapping[index]) {
    taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->sendMessage(BUILD_PATH(MQTT_PATH_TEMPS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(TEMP_OFFSET + index)), getTemperature(index));
  } else {
    LOG_PRINTLN(F("Invalid sensors or mapping"));
  }
}
