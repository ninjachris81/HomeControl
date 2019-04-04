#include "EspMQTTClient.h"
#include "credentials.h"
#include "config.h"
#include "constants.h"

#define BUILD_PATH(subpath) MQTT_BASE_PATH + String(MQTT_PATH_SEP) + subpath

void onConnectionEstablished();

EspMQTTClient client(
  WIFI_SSID,                 // Wifi ssid
  WIFI_PASS,                 // Wifi password
  onConnectionEstablished,// MQTT connection established callback
  MQTT_SERVER                    // MQTT broker ip
);

#define RELAY_COUNT 4
bool states[RELAY_COUNT];

void setup () 
{
  Serial.begin (115200);
  for (uint8_t i=0;i<RELAY_COUNT;i++) {
    states[i] = false;
  }
}

void sendError(String errorDesc) {
  client.publish(BUILD_PATH(MQTT_PATH_ERRORS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(MQTT_PATH_ERRORS_RELAYS)), "s" + errorDesc);
}

void setState(uint8_t i, const String &payload) {
  if (payload.length()==2) {
    if (payload.charAt(0)==MQTT_ID_BOOL) {
      byte relayData[4];

      states[i] = (payload.charAt(1)=='1');

      relayData[0] = 0xA0;
      relayData[1] = i+1;
      relayData[2] = states[i] ? 0x01 : 0x00;
      relayData[3] = relayData[0] + relayData[1] + relayData[2];
      
      Serial.write(relayData, sizeof(relayData));
      
      sendState(i);
    } else {
      // invalid type
      sendError("Invalid type");
    }
  } else {
    // invalid payload length
    sendError("Invalid payload length");
  }
}

void sendState(uint8_t i) {
  client.publish(BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_VAL + String(MQTT_PATH_SEP) + String(i)), states[i] ? "b1" : "b0");
}

void onBroadcast() {
  for (uint8_t i=0;i<RELAY_COUNT;i++) {
    sendState(i);
  }  
}

void onConnectionEstablished() {
  // TODO: use lambda with capture
  client.subscribe(BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_SET + String("/0")), [](const String & payload) {
      setState(0, payload);
  });
  client.subscribe(BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_SET + String("/1")), [](const String & payload) {
      setState(1, payload);
  });
  client.subscribe(BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_SET + String("/2")), [](const String & payload) {
      setState(2, payload);
  });
  client.subscribe(BUILD_PATH(MQTT_PATH_RELAYS + String(MQTT_PATH_SEP) + MQTT_SET + String("/3")), [](const String & payload) {
      setState(3, payload);
  });


  client.subscribe(BUILD_PATH(MQTT_PATH_BC), [](const String & payload) {
    if (payload==MQTT_BC_CMD_BC_ALL || payload==MQTT_PATH_RELAYS) onBroadcast();
  });

  /*
  client.publish("mytopic/test", "This is a message");

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/test", "This is a message sent 5 seconds later");
  });
  */
}

void loop()
{
  client.loop();
  delay(100);
}
