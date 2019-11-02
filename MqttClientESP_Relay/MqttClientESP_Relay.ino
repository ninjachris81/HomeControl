#include "ESPConfigurations.h"

#include "constants.h"

#include "TaskManager.h"
#include "MqttController.h"
#include "RelayController.h"
#include "TempController.h"

#include <ArduinoOTA.h>

TaskManager taskManager;
MqttController mqttController;

#if RELAY_COUNT > 0
  RelayController relayController;
#endif

#if TEMP_COUNT > 0
  TempController tempController;
#endif

void onConnectionEstablished() {}   // legacy

void initOTA() {
    ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    LOG_PRINTLN("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    LOG_PRINTLN("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      LOG_PRINTLN("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      LOG_PRINTLN("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      LOG_PRINTLN("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      LOG_PRINTLN("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      LOG_PRINTLN("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void setup () {
  taskManager.registerTask(&mqttController);
#if RELAY_COUNT > 0
  taskManager.registerTask(&relayController);
#endif
#if TEMP_COUNT > 0
  taskManager.registerTask(&tempController);
#endif

  taskManager.init();

  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  uint8_t bssid[] = WIFI_BSSID;
  WiFi.begin(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL, bssid, false);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    LOG_PRINTLN("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  if (WIFI_OTA_SUPPORT) initOTA();
}

void loop()
{
  taskManager.update();
  if (WIFI_OTA_SUPPORT) ArduinoOTA.handle();
  delay(10);
}
