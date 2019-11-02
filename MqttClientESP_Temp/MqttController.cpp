#include "MqttController.h"
#include "ESPConfigurations.h"
#include <LogHelper.h>

MqttController* MqttController::instance = NULL;

MqttController::MqttController(bool thisAutoSubscribeToBC, bool thisSendStartup) : AbstractIntervalTask(MQTT_INTERVAL_MS), autoSubscribeToBC(thisAutoSubscribeToBC), sendStartup(thisSendStartup) {
  
}

MqttController::~MqttController() {
  delete mqttClient;
}

void MqttController::init() {
  WiFi.persistent(false);

  instance = this;
  
  mqttClient = new EspMQTTClient(
    WIFI_SSID,                 // Wifi ssid
    WIFI_PASS,                 // Wifi password
    &MqttController::onConnectionEstablishedStatic,// MQTT connection established callback
    MQTT_SERVER,                    // MQTT broker ip
    MQTT_PORT,
    "",
    "",
    DEVICE_NAME,
    false
  );
}

void MqttController::update() {
  mqttClient->loop();
}

void MqttController::onConnectionEstablishedStatic() {
  instance->onConnectionEstablished();
}

void MqttController::onConnectionEstablished() {
  if (autoSubscribeToBC) {
    mqttClient->subscribe(-1, -1, BUILD_PATH(MQTT_PATH_BC), &MqttController::onBroadcastReceivedStatic);
  }
  
  for (uint8_t i=0;i<callbackHandlerCount;i++) callbackHandlers[i]->onConnected();

  if (sendStartup) {
    sendLog(MQTT_PATH_LOGS_TYPE_STARTUP, "");
  }
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
        for (uint8_t i=0;i<callbackHandlerCount;i++) {
          if (callbackHandlers[i]->getID()==endpoint) {
            callbackHandlers[i]->onIntReceived(index, message.substring(1).toInt());
            break;
          }
        }

        break;
      case MQTT_ID_DOUBLE:
        for (uint8_t i=0;i<callbackHandlerCount;i++) {
          if (callbackHandlers[i]->getID()==endpoint) {
            callbackHandlers[i]->onDoubleReceived(index, message.substring(1).toFloat());
            break;
          }
        }

        break;
      case MQTT_ID_STRING:
        for (uint8_t i=0;i<callbackHandlerCount;i++) {
          if (callbackHandlers[i]->getID()==endpoint) {
            callbackHandlers[i]->onStringReceived(index, message.substring(1));
            break;
          }
        }
        break;
      default:
      sendError(F("Invalid type"), message.charAt(0));
      break;
    }
    
  } else {
      sendError(F("Invalid size"), message.length());
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

void MqttController::sendError(String errorDesc, int value) {
  errorDesc += " " + String(value);
  sendError(errorDesc);
}

void MqttController::sendError(String errorDesc) {
  if (mqttClient->isConnected()) {
    sendLog(MQTT_PATH_LOGS_TYPE_ERROR, errorDesc);
  }
}

void MqttController::sendLog(uint8_t type, String desc) {
  if (mqttClient->isConnected()) {
    desc = String(DEVICE_NAME) + String(MQTT_LOG_SOURCE_DIV) + desc;
    sendMessage(BUILD_PATH(MQTT_PATH_LOGS + String(MQTT_PATH_SEP) + MQTT_SET + String(MQTT_PATH_SEP) + String(type)), desc);
  }
}

void MqttController::sendLog(uint8_t type, String desc, int value) {
  desc += " " + String(value);
  sendLog(type, desc);
}

void MqttController::sendMessage(String path, String payload) {
  if (mqttClient->isConnected()) {
    payload = String(MQTT_ID_STRING) + payload;
    mqttClient->publish(path, payload);
  }
}

void MqttController::sendMessage(String path, bool payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, String(MQTT_ID_BOOL) + (payload ? String("1") : String("0")));
  }
}

void MqttController::sendMessage(String path, int payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, String(MQTT_ID_INTEGER) + payload);
  }
}

void MqttController::sendMessage(String path, double payload) {
  if (mqttClient->isConnected()) {
    mqttClient->publish(path, String(MQTT_ID_DOUBLE) + String(payload));
  }
}
