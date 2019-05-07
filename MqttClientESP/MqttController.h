#ifndef MQTTCONTROLLER_H
#define MQTTCONTROLLER_H

#include <AbstractIntervalTask.h>
#include "credentials.h"
#include "config.h"
#include "constants.h"
#include "EspMQTTClient.h"

#define MQTT_INTERVAL_MS 100
#define MAX_MQTT_EVENT_CB_HANDLERS 8

#define BUILD_PATH(subpath) MQTT_BASE_PATH + String(MQTT_PATH_SEP) + subpath

class MqttController : public AbstractIntervalTask {
public:
  MqttController();
  ~MqttController();

  class MqttEventCallbackHandler {
    public:
      virtual void onConnected() = 0;
      
      virtual void onBroadcast() = 0;
      virtual String getBroadcastPath() = 0;
      virtual int getID() = 0;

      virtual void onStringReceived(int index, String value) = 0;
      virtual void onBoolReceived(int index, bool value) = 0;
      virtual void onIntReceived(int index, int value) = 0;
      virtual void onDoubleReceived(int index, double value) = 0;
  };
  
  void init();

  void update();

  static void onConnectionEstablishedStatic();
  void onConnectionEstablished();

  static void onBroadcastReceivedStatic(const int endpoint, const int index, const String &message);
  void onBroadcastReceived(const int endpoint, const int index, const String &message);

  static void onMessageReceivedStatic(const int endpoint, const int index, const String &message);
  void onMessageReceived(const int endpoint, const int index, const String &message);

  void registerHandler(MqttEventCallbackHandler* handler);

  void subscribePath(const int endpoint, const int index, String path);

  void sendError(String errorDesc);
  void sendError(String errorDesc, int value);

  void sendMessage(String path, String payload);
  void sendMessage(String path, bool payload);
  void sendMessage(String path, int payload);
  void sendMessage(String path, double payload);

private:
  static MqttController* instance;
  EspMQTTClient *mqttClient;

  MqttEventCallbackHandler* callbackHandlers[MAX_MQTT_EVENT_CB_HANDLERS];
  uint8_t callbackHandlerCount = 0;
};


#endif
