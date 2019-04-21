#include "MqttController.h"

MqttController* MqttController::instance = NULL;

MqttController::MqttController() : AbstractIntervalTask(MQTT_INTERVAL_MS) {
  
}

MqttController::~MqttController() {
  delete mqttClient;
}

void MqttController::init() {
  instance = this;
  
  mqttClient = new EspMQTTClient(
    WIFI_SSID,                 // Wifi ssid
    WIFI_PASS,                 // Wifi password
    &MqttController::onConnectionEstablishedStatic,// MQTT connection established callback
    MQTT_SERVER                    // MQTT broker ip
  );
}

void MqttController::update() {
  mqttClient->loop();
}

void MqttController::onConnectionEstablishedStatic() {
  instance->onConnectionEstablished();
}

void MqttController::onConnectionEstablished() {
  mqttClient->subscribe(-1, -1, BUILD_PATH(MQTT_PATH_BC), &MqttController::onBroadcastReceivedStatic);
  for (uint8_t i=0;i<callbackHandlerCount;i++) callbackHandlers[i]->onConnected();
}

void MqttController::onBroadcastReceivedStatic(const int endpoint, const int index, const String &message) {
  instance->onBroadcastReceived(endpoint, index, message);
}

void MqttController::onBroadcastReceived(const int endpoint, const int index, const String &message) {
  if (message.charAt(0)==MQTT_ID_STRING) {
    if (message.substring(1)==MQTT_BC_CMD_BC_ALL) {
      for (uint8_t i=0;i<callbackHandlerCount;i++) callbackHandlers[i]->onBroadcast();  
    } else {
      for (uint8_t i=0;i<callbackHandlerCount;i++) {
        if (message.substring(1)==callbackHandlers[i]->getBroadcastPath()) {
          callbackHandlers[i]->onBroadcast();  
        }
      }
    }
  } else {
    sendError(F("Invalid broadcast type"));
  }
}

void MqttController::onMessageReceivedStatic(const int endpoint, const int index, const String &message) {
  instance->onMessageReceived(endpoint, index, message);
}

void MqttController::onMessageReceived(const int endpoint, const int index, const String &message) {
  if (message.length()>=MQTT_MIN_MSG_SIZE) {
    switch(message.charAt(0)) {
      case MQTT_ID_BOOL:
        for (uint8_t i=0;i<callbackHandlerCount;i++) {
          if (callbackHandlers[i]->getID()==endpoint) {
            callbackHandlers[i]->onBoolReceived(index, message.charAt(1)=='1');
            break;
          }
        }
      break;
      case MQTT_ID_INTEGER:
      break;
      case MQTT_ID_DOUBLE:
      break;
      case MQTT_ID_STRING:
      break;
      default:
      sendError(F("Invalid type"));
      break;
    }
    
  } else {
      sendError(F("Invalid size"));
  }
}

void MqttController::registerHandler(MqttEventCallbackHandler* handler) {
  callbackHandlers[callbackHandlerCount] = handler;
  callbackHandlerCount++;
}

void MqttController::subscribePath(const int endpoint, const int index, String path) {
  if (mqttClient->isConnected()) {
    mqttClient->subscribe(endpoint, index, path, &MqttController::onMessageReceivedStatic);
  }
}

void MqttController::sendError(String errorDesc) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(BUILD_PATH(MQTT_PATH_ERRORS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_ERRORS_RELAYS)), "s" + errorDesc);
  }
}

void MqttController::sendMessage(String path, String payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, "s" + payload);
  }
}

void MqttController::sendMessage(String path, bool payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, payload ? "b1" : "b0");
  }
}

void MqttController::sendMessage(String path, int payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, "i" + payload);
  }
}

void MqttController::sendMessage(String path, double payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, "d" + String(payload));
  }
}
