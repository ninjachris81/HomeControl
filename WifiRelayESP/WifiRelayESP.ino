#include "EspMQTTClient.h"
#include "credentials.h"
#include "config.h"
#include "constants.h"

void onConnectionEstablished();

EspMQTTClient client(
  WIFI_SSID,                 // Wifi ssid
  WIFI_PASS,                 // Wifi password
  onConnectionEstablished,// MQTT connection established callback
  MQTT_SERVER                    // MQTT broker ip
);

#define RELAY_COUNT 4

byte relON[]  = {0xA0, 0x01, 0x01, 0xA2};
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1};
byte rel2ON[]  = {0xA0, 0x02, 0x01, 0xA3};
byte rel2OFF[] = {0xA0, 0x02, 0x00, 0xA2};

void setup () 
{
  Serial.begin (115200);
}

void onConnectionEstablished()
{

  for (uint8_t i=0;i<RELAY_COUNT;i++) {
    // Subscribe to "mytopic/test" and display received message to Serial
    client.subscribe(MQTT_BASE_PATH + i, [](const String & payload) {
      Serial.println(payload);
    });
  }

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message");

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/test", "This is a message sent 5 seconds later");
  });
}

void loop()
{
  client.loop();
}
